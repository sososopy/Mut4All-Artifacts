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

pub struct Insert_DynStar_Cast_338;

impl Mutator for Insert_DynStar_Cast_338 {
    fn name(&self) -> &str {
        "Insert_DynStar_Cast_338"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.last().unwrap().ident.to_string() == "PointerLike" {
                                let dyn_star_cast = quote! {
                                    let _: dyn * #type_path = #pat_type.pat;
                                };
                                let mut new_block = func.block.clone();
                                new_block.stmts.insert(0, Stmt::Expr(syn::Expr::Verbatim(dyn_star_cast), None));
                                func.block = new_block;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if type_path.path.segments.last().unwrap().ident.to_string() == "PointerLike" {
                                        let dyn_star_cast = quote! {
                                            let _: dyn * #type_path = #pat_type.pat;
                                        };
                                        let mut new_block = func.block.clone();
                                        new_block.stmts.insert(0, Stmt::Expr(syn::Expr::Verbatim(dyn_star_cast), None));
                                        func.block = new_block;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a dyn* cast expression to a variable or a function call return value, where the type of the variable or return value implements the PointerLike trait. This transformation tests the compiler's handling of dyn* casts and trait objects, potentially triggering bugs related to the F-dyn_star feature."
    }
}