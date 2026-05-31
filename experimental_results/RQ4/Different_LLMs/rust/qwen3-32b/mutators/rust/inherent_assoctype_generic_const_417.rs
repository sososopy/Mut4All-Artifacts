use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprBinary, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Inherent_AssocType_Generic_Const_417;

impl Mutator for Inherent_AssocType_Generic_Const_417 {
    fn name(&self) -> &str {
        "Inherent_AssocType_Generic_Const_417"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let impl_generics = &impl_item.generics;
                let has_const_params = impl_generics.params.iter().any(|param| matches!(param, GenericParam::Const(_)));
                if has_const_params {
                    let n_ident = impl_generics.params.iter()
                        .find(|param| matches!(param, GenericParam::Const(_)))
                        .and_then(|param| {
                            if let GenericParam::Const(c) = param {
                                Some(c.ident.clone())
                            } else {
                                None
                            }
                        });
                    if let Some(n) = n_ident {
                        for impl_item in &mut impl_item.items {
                            if let ImplItem::Type(type_alias) = impl_item {
                                let new_const_param = parse_quote!(const M: usize);
                                type_alias.generics.params.push(new_const_param);
                                if let Type::Path(type_path) = &mut type_alias.ty {
                                    for segment in &mut type_path.path.segments {
                                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    let new_expr = parse_quote! { #n + M };
                                                    *expr = new_expr;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                type_alias.generics.where_clause = Some(parse_quote! {
                                    where [(); #n + M]:
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a generic const parameter to inherent associated types within impl blocks. It modifies the associated type's const expression to combine the struct's const parameter with the new generic parameter and adds an array length constraint in a where clause. This stresses the const parameter substitution logic during type resolution and may expose bugs in handling complex const expressions in associated types."
    }
}