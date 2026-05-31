use proc_macro2::Span;
use std::error::Error;
use syn::{
    Field, Fields, FieldsNamed, Generics, Ident, Item, ItemStruct,
    Visibility,
    punctuated::Punctuated,
    token::Brace,
};

use crate::{
    do_gen_name,
    strategies::syn::strategy::FuzzerStrategyImpl,
    gen_token, register_strategy,
    strategies::syn::strategy::{
        consts::{FIELDS_R, FN_NAME_R},
        info::env::{ASTEnv, current_env},
    },
    use_nodetype,
    util::{gen_alpha, glob_range},
};

use_nodetype!(syn, Item);
use_nodetype!(syn, Block);
use_nodetype!(syn, Type);

#[derive(Default)]
pub struct ItemStructStrategy;
impl FuzzerStrategyImpl<Item> for ItemStructStrategy {
    fn do_gen(&mut self) -> Result<Item, Box<dyn Error>> {
        let struct_name = gen_alpha(glob_range(FN_NAME_R));
        {
            let env_g = current_env();
            let mut env_l = env_g.write().unwrap();
            env_l.insert_struct(&struct_name);
        }
        let ident = Ident::new(&struct_name, Span::call_site());
        let mut fields = Punctuated::default();
        for _ in 0..glob_range(FIELDS_R) {
            let field_name = gen_alpha(glob_range(FN_NAME_R));
            let field_ident = Ident::new(&field_name, Span::call_site());
            let field = Field {
                attrs: vec![],
                vis: Visibility::Inherited,
                mutability: syn::FieldMutability::None,
                ident: Some(field_ident),
                colon_token: Some(gen_token!(:)),
                ty: do_gen_name!(Type)?.do_gen()?.into(),
            };
            fields.push(field);
        }
        let fields = Fields::Named(FieldsNamed {
            brace_token: Brace::default(),
            named: fields,
        });
        let res = ItemStruct {
            attrs: vec![],
            vis: Visibility::Inherited,
            struct_token: gen_token!(struct),
            ident,
            generics: Generics::default(), // todo: add generics
            fields,
            semi_token: None,
        };
        Ok(Item::Struct(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 1.
    }
}

register_strategy!(ItemStructStrategy, Item, Item::Struct(..));
