use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Struct_With_Uninhabited_Type_215;

impl Mutator for Modify_Struct_With_Uninhabited_Type_215 {
    fn name(&self) -> &str {
        "Modify_Struct_With_Uninhabited_Type_215"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut modified = false;
                for field in &mut item_struct.fields {
                    if let syn::Type::Never(_) = field.ty {
                        field.ty = parse_quote!(u8);
                        modified = true;
                    }
                }
                if modified {
                    let struct_name = &item_struct.ident;
                    let use_func_name = Ident::new(&format!("use_{}", struct_name), Span::call_site());
                    let new_func: ItemFn = parse_quote! {
                        fn #use_func_name() {
                            let instance = #struct_name { x: 0, y: 0 };
                            println!("Struct instance: x = {}, y = {}", instance.x, instance.y);
                        }
                    };
                    file.items.push(Item::Fn(new_func));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}