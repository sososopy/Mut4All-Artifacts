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

pub struct Replace_Const_With_Variable_415;

impl Mutator for Replace_Const_With_Variable_415 {
    fn name(&self) -> &str {
        "Replace_Const_With_Variable_415"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                if let syn::Generics::Where(clause) = &struct_item.generics {
                    for predicate in &mut clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &*predicate_type.bounded_ty {
                                if let Some(constant) = find_constant(type_path) {
                                    let variable_name = Ident::new(&format!("{}_var", constant), Span::call_site());
                                    let variable_type = get_type(constant);
                                    let new_expr = Expr::Path(ExprPath {
                                        attrs: vec![],
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: variable_name,
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                        qself: None,
                                    });
                                    replace_constant_with_variable(file, constant, new_expr, variable_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant values used in generic type parameters with variables. It applies to code constructs where a constant value is used as a type parameter in a generic type. This transformation tests how the compiler handles the substitution of constants with variables in generic types, which could expose bugs related to type checking and substitution."
    }
}

fn find_constant(type_path: &TypePath) -> Option<Ident> {
    for segment in &type_path.path.segments {
        if let Some(constant) = segment.ident.to_string().strip_prefix("const_") {
            return Some(Ident::new(constant, Span::call_site()));
        }
    }
    None
}

fn get_type(constant: &Ident) -> Type {
    let mut rng = thread_rng();
    let types = vec![Type::Path(TypePath {
        qself: None,
        path: SynPath {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![PathSegment {
                ident: Ident::new("usize", Span::call_site()),
                arguments: PathArguments::None,
            }]),
        },
    }), Type::Path(TypePath {
        qself: None,
        path: SynPath {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![PathSegment {
                ident: Ident::new("isize", Span::call_site()),
                arguments: PathArguments::None,
            }]),
        },
    }), Type::Path(TypePath {
        qself: None,
        path: SynPath {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![PathSegment {
                ident: Ident::new("bool", Span::call_site()),
                arguments: PathArguments::None,
            }]),
        },
    }), Type::Path(TypePath {
        qself: None,
        path: SynPath {
            leading_colon: None,
            segments: Punctuated::from_iter(vec![PathSegment {
                ident: Ident::new("char", Span::call_site()),
                arguments: PathArguments::None,
            }]),
        },
    })];
    types.choose(&mut rng).unwrap().clone()
}

fn replace_constant_with_variable(file: &mut File, constant: Ident, new_expr: Expr, variable_type: Type) {
    for item in &mut file.items {
        if let syn::Item::Struct(struct_item) = item {
            if let syn::Generics::Where(clause) = &struct_item.generics {
                for predicate in &mut clause.predicates {
                    if let WherePredicate::Type(predicate_type) = predicate {
                        if let Type::Path(type_path) = &*predicate_type.bounded_ty {
                            if type_path.path.segments.iter().any(|segment| segment.ident == constant) {
                                *predicate_type.bounded_ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new(&format!("{}_var", constant), Span::call_site()),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    file.items.push(Item::Const(syn::ItemConst {
        attrs: vec![],
        vis: Visibility::Inherited { span: Span::call_site() },
        const_token: Default::default(),
        ident: Ident::new(&format!("{}_var", constant), Span::call_site()),
        eq_token: Default::default(),
        expr: Box::new(new_expr),
        semi_token: Default::default(),
    }));
}