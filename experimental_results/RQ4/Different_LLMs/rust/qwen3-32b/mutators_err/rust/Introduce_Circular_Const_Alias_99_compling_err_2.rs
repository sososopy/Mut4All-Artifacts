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

pub struct Introduce_Circular_Const_Alias_99;

impl Mutator for Introduce_Circular_Const_Alias_99 {
    fn name(&self) -> &str {
        "Introduce_Circular_Const_Alias_99"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits_with_const = Vec::new();
        let mut modified_traits = HashSet::new();

        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                for param in &trait_def.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        traits_with_const.push((trait_def.ident.clone(), const_param.ty.clone()));
                        modified_traits.insert(trait_def.ident.clone());
                        break;
                    }
                }
            }
        }

        for item in &mut file.items {
            if let syn::Item::Type(type_alias) = item {
                if let syn::Type::Path(type_path) = &mut *type_alias.ty {
                    let first_segment = type_path.path.segments.first_mut().unwrap();
                    if let syn::PathArguments::AngleBracketed(angle_args) = &mut first_segment.arguments {
                        let trait_name = first_segment.ident.to_string();
                        if traits_with_const.iter().any(|(t_name, _)| t_name.as_str() == trait_name) && !modified_traits.contains(&first_segment.ident) {
                            for trait_item in &mut file.items {
                                if let syn::Item::Trait(trait_def) = trait_item {
                                    if trait_def.ident == first_segment.ident {
                                        for param in &mut trait_def.generics.params {
                                            if let syn::GenericParam::Const(const_param) = param {
                                                let alias_name = &type_alias.ident;
                                                let new_type = syn::Type::Path(syn::TypePath {
                                                    qself: None,
                                                    path: syn::Path::from(alias_name.clone()),
                                                });
                                                const_param.ty = new_type;
                                                modified_traits.insert(trait_def.ident.clone());
                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            for arg in angle_args.args.iter_mut() {
                                if let syn::GenericArgument::Const(expr) = arg {
                                    if let syn::Expr::Lit(lit) = expr {
                                        if let syn::Lit::Int(int_lit) = lit.lit {
                                            let original_value = int_lit.base10_parse::<u64>().unwrap();
                                            let left = original_value - 1;
                                            let right = 1;
                                            let new_expr = syn::parse_quote! { $left + $right };
                                            *expr = new_expr;
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
        "The mutation operator introduces circular dependencies between trait const parameters and type aliases by replacing the const parameter's type with the alias and the alias's const value with a computed expression. This creates conflicting resolution paths and tests the compiler's query system under circular constraints."
    }
}