use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Insert_Deref_Cast_DynTrait_To_Ptr_337;

impl Mutator for Insert_Deref_Cast_DynTrait_To_Ptr_337 {
    fn name(&self) -> &str {
        "Insert_Deref_Cast_DynTrait_To_Ptr_337"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Pat::Ident(ref ident_pat) = *pat_type.pat {
                            if let Type::Reference(type_ref) = &*pat_type.ty {
                                if let Type::TraitObject(trait_obj) = &*type_ref.elem {
                                    if trait_obj.dyn_token.is_some() {
                                        if let Some(first_trait) = trait_obj.bounds.iter().find_map(|bound| {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                Some(trait_bound.path.clone())
                                            } else {
                                                None
                                            }
                                        }) {
                                            let expr: Expr = parse_quote! {
                                                *#ident_pat as *const dyn #first_trait
                                            };
                                            let stmt = Stmt::Expr(expr, None);
                                            func.block.stmts.insert(0, stmt);
                                            break;
                                        }
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
        "The mutation operator inserts a dereference and cast from `&dyn Trait` to `*const dyn Trait` within functions that accept a `&dyn Trait` parameter. This introduces a raw pointer cast in a context expecting coercion, potentially triggering type system inconsistencies, invalid memory accesses, or compiler errors related to trait object handling and pointer safety."
    }
}