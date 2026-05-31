use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, path::Path, process::Command};
use syn::{
    Expr, File, GenericParam, Ident, Item, ItemFn, parse_quote,
    token, Type, TypePath, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Change_Const_Generic_Param_To_Int_292;

impl Mutator for Change_Const_Generic_Param_To_Int_292 {
    fn name(&self) -> &str {
        "Change_Const_Generic_Param_To_Int_292"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                let mut has_const_param = false;

                for param in &mut generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if let syn::Type::Path(type_path) = &const_param.ty {
                            if type_path.path.segments.last().unwrap().ident == "MyStr" {
                                const_param.ty = syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("usize", Span::call_site())),
                                });
                                has_const_param = true;
                            }
                        }
                    }
                }

                if has_const_param {
                    func.sig.output = syn::ReturnType::Type(
                        token::RArrow { spans: [Span::call_site(), Span::call_site()] },
                        Box::new(syn::Type::Path(syn::TypePath {
                            qself: None,
                            path: syn::Path::from(Ident::new("usize", Span::call_site())),
                        })),
                    );

                    let mut new_block = func.block.clone();
                    new_block.stmts.clear();
                    new_block.stmts.push(parse_quote! { S + 1; });
                    func.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}