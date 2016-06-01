var circular_buffer = {};
circular_buffer.create = function(params){
	var obj = {};
	if(params == undefined){
		params = {};
	}
	this._set_default_params(params);
	this._init_obj(obj,params);
	return obj;
};
circular_buffer._set_default_params = function(params){
	if(params.Max == undefined ||
		typeof(params.Max) != "number" ||
		params.Max < 1){
		params.Max = 100;
	}
};
circular_buffer._init_obj = function(obj,params){
	obj._records = params.Max;
	//不存在 標記
	obj.npos = -1;
	//環緩衝區
	obj._datas = [];
	obj._begin = obj.npos;
	obj._end = 0;
	//[begin,end)
	//返回記錄 大小
	obj.size = function(){
		if(this._begin == this.npos){
			return 0;
		}else if(this._end > this._begin){
			return this._end - this._begin;
		}
		return this._end + this._records - this._begin;
	};
	//返回 逆向 迭代器
	obj._iterator = 1;
	obj._riterator = 2;
	obj.rbegin = function(){
		var obj = {};
		//設置迭代器 類型
		obj._itype = this._riterator;

		//初始化 偏移
		if(this._begin == this.npos){
			obj._pos = this.npos;
			return obj;
		}
		obj._pos = this._end - 1;
		if(obj._pos < 0){
			obj._pos = this._records - 1;
		}
		return obj;
	};
	//返回迭代器
	obj.begin = function(){
		var obj = {};
		//設置迭代器 類型
		obj._itype = this._iterator;

		//初始化 偏移
		obj._pos = this._begin;
		return obj;
	};
	//返回迭代器 是否可 移動
	obj.has_next = function(iterator){
		if(iterator._itype == this._riterator){
			return this._ri_has_next(iterator);
		}else if(iterator._itype == this._iterator){
			return this._i_has_next(iterator);
		}
		return false;
	};
	obj.next = function(iterator){
		if(iterator._itype == this._riterator){
			return this._ri_next(iterator);
		}else if(iterator._itype == this._iterator){
			return this._i_next(iterator);
		}
		return false;
	};
	obj._ri_has_next = function(iterator){
		if(iterator._pos == this.npos){
			return false;
		}
		return iterator._pos != this._begin;
	};
	obj._i_has_next = function(iterator){
		if(iterator._pos == this.npos){
			return false;
		}
		var pos = iterator._pos + 1;
		if(pos == this._records){
			pos = 0;
		}
		return pos != this._end;
	};
	obj._ri_next = function(iterator){
		if(!this._ri_has_next(iterator)){
			iterator._pos = this.npos;
			return false;
		}
		if(iterator._pos > this._begin){
			--iterator._pos;
		}else{
			if(iterator._pos == 0){
				iterator._pos = this._records - 1;
			}else{
				--iterator._pos;
			}
		}
		return true;
	};
	obj._i_next = function(iterator){
		if(!this._i_has_next(iterator)){
			iterator._pos = this.npos;
			return false;
		}
		++iterator._pos;
		if(iterator._pos == this._records){
			iterator._pos = 0;
		}
		return true;
	};

	obj.get_data = function(iterator){
		if(iterator._pos == this.npos){
			return null;
		}
		return this._datas[iterator._pos];
	};


	obj.push_back = function(data){
		if(this._datas.length < this._records){
			this._datas.push(data);
			++this._end;
			if(this._begin == this.npos){
				this._begin = 0;
			}
			if(this._end == this._records){
				this._end = 0;
			}
			return;
		}

		var pos = this._end++;
		this._datas[pos] = data;
		if(this._end == this._records){
			this._end = 0;
		}
		if(pos == this._begin){
			++this._begin;
			if(this._begin == this._records){
				this._begin = 0;
			}
		}
	};
	obj.clear = function(){
		this._datas = [];
		this._begin = this.npos;
		this._end = 0;
	};
};