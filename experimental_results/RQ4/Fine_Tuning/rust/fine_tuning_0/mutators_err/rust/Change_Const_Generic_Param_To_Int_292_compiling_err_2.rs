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
                        if let syn::Type::Reference(type_ref) = const_param.ty.as_ref() {
                            if let syn::Type::Path(type_path) = type_ref.elem.as_ref() {
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
                    new_block.stmts.push(parse_quote! { S + 1 });
                    func.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}