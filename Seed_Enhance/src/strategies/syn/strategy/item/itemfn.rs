use proc_macro2::Span;
use std::error::Error;
use syn::{
    Generics, Ident, Item, ItemFn, ReturnType, Signature, Visibility, punctuated::Punctuated,
    token::Paren,
};

use crate::{
    do_gen_name, gen_token, register_strategy,
    strategies::syn::strategy::FuzzerStrategyImpl,
    strategies::syn::strategy::{
        consts::{FN_ASYNC_B, FN_CONST_B, FN_NAME_R, FN_UNSAFE_B},
        info::env::{ASTEnv, current_env},
    },
    use_nodetype,
    util::{gen_alpha, glob_range},
};

use_nodetype!(syn, Item);
use_nodetype!(syn, Block);

pub struct ItemFnStrategy;
impl Default for ItemFnStrategy {
    fn default() -> Self {
        Self
    }
}
impl FuzzerStrategyImpl<Item> for ItemFnStrategy {
    fn do_gen(&mut self) -> Result<Item, Box<dyn Error>> {
        let fn_name = gen_alpha(glob_range(FN_NAME_R));
        {
            let env_g = current_env();
            let mut env_l = env_g.write().unwrap();
            env_l.insert_func(&fn_name);
        }
        let ident = Ident::new(&fn_name, Span::call_site());
        let sig = Signature {
            constness: if FN_CONST_B > glob_range(0. ..1.) {
                Some(gen_token!(const))
            } else {
                None
            },
            asyncness: if FN_ASYNC_B > glob_range(0. ..1.) {
                Some(gen_token!(async))
            } else {
                None
            },
            unsafety: if FN_UNSAFE_B > glob_range(0. ..1.) {
                Some(gen_token!(unsafe))
            } else {
                None
            },
            abi: None,
            fn_token: gen_token!(fn),
            ident,
            generics: Generics::default(), //todo
            paren_token: Paren::default(), //todo
            inputs: Punctuated::default(), //todo
            variadic: None,
            output: ReturnType::Default,
        };
        let res = ItemFn {
            attrs: vec![],
            vis: Visibility::Inherited,
            sig,
            block: do_gen_name!(Block)?.do_gen()?.into(),
        };
        Ok(Item::Fn(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

pub fn do_gen_main() -> Result<Item, Box<dyn Error>> {
    let ident = Ident::new("main", Span::call_site());
    let sig = Signature {
        constness: if FN_CONST_B > glob_range(0. ..1.) {
            Some(gen_token!(const))
        } else {
            None
        },
        asyncness: if FN_ASYNC_B > glob_range(0. ..1.) {
            Some(gen_token!(async))
        } else {
            None
        },
        unsafety: if FN_UNSAFE_B > glob_range(0. ..1.) {
            Some(gen_token!(unsafe))
        } else {
            None
        },
        abi: None,
        fn_token: gen_token!(fn),
        ident,
        generics: Generics::default(), //todo
        paren_token: Paren::default(), //todo
        inputs: Punctuated::default(), //todo
        variadic: None,
        output: ReturnType::Default,
    };
    let res = ItemFn {
        attrs: vec![],
        vis: Visibility::Inherited,
        sig,
        block: do_gen_name!(Block)?.do_gen()?.into(),
    };
    Ok(Item::Fn(res))
}

register_strategy!(ItemFnStrategy, Item, Item::Fn(..));
