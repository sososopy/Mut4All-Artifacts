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

pub struct Modify_Function_Pointer_Const_Generic_406;

impl Mutator for Modify_Function_Pointer_Const_Generic_406 {
    fn name(&self) -> &str {
        "Modify_Function_Pointer_Const_Generic_406"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        let mut updated_structs = Vec::new();

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(GenericParam::Const(const_param)) = item_struct.generics.params.first() {
                    if let Type::Path(type_path) = &const_param.ty {
                        if type_path.path.segments.last().unwrap().ident == "fn" {
                            let new_ident = Ident::new("SIZE", Span::call_site());
                            let new_type: Type = parse_quote!(usize);
                            let new_generic_param = GenericParam::Const(ConstParam {
                                attrs: vec![],
                                const_token: const_param.const_token,
                                ident: new_ident.clone(),
                                colon_token: const_param.colon_token,
                                ty: new_type,
                                eq_token: None,
                                default: None,
                            });
                            let mut new_params = item_struct.generics.params.clone();
                            new_params.clear();
                            new_params.push(new_generic_param);
                            let mut updated_struct = item_struct.clone();
                            updated_struct.generics.params = new_params;
                            updated_structs.push((updated_struct, const_param.ident.clone(), new_ident));
                        } else {
                            new_items.push(item.clone());
                        }
                    } else {
                        new_items.push(item.clone());
                    }
                } else {
                    new_items.push(item.clone());
                }
            } else {
                new_items.push(item.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for (updated_struct, old_ident, new_ident) in &updated_structs {
                    if let Some(GenericParam::Const(impl_const_param)) = item_impl.generics.params.first_mut() {
                        if impl_const_param.ident == *old_ident {
                            impl_const_param.ident = new_ident.clone();
                        }
                    }
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            for stmt in &mut func.block.stmts {
                                if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                                    if let Expr::Path(expr_path) = &*expr_call.func {
                                        if expr_path.path.segments.last().unwrap().ident == *old_ident {
                                            *stmt = parse_quote! {
                                                SIZE as u32
                                            };
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for (updated_struct, _, _) in updated_structs {
            new_items.push(Item::Struct(updated_struct));
        }

        file.items = new_items;
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies structs with const generic parameters using function pointers and modifies them to use a numeric constant instead. It updates the struct definition and all references within its implementation to use the new numeric constant, thus testing the handling of const generics in the Rust compiler."
    }
}