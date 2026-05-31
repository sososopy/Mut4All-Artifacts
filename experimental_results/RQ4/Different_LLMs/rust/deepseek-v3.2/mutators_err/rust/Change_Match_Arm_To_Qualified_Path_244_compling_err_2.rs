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

pub struct Change_Match_Arm_To_Qualified_Path_244;

impl Mutator for Change_Match_Arm_To_Qualified_Path_244 {
    fn name(&self) -> &str {
        "Change_Match_Arm_To_Qualified_Path_244"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MatchArmVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match arm patterns that are tuple struct or enum variant patterns. It scans for all enum definitions in scope, then for each match arm pattern, it looks for another enum variant with the same name but different arity. If found, it mutates the pattern to use a fully qualified path (::Enum::Variant) from the different enum, creating a type mismatch in pattern arity. This transformation aims to stress the compiler's pattern analysis logic, particularly in handling qualified paths and arity checking, potentially triggering internal compiler inconsistencies."
    }
}

struct MatchArmVisitor {
    enums_in_scope: Vec<EnumInfo>,
}

struct EnumInfo {
    name: Ident,
    variants: Vec<VariantInfo>,
}

struct VariantInfo {
    name: Ident,
    arity: usize,
}

impl MatchArmVisitor {
    fn new() -> Self {
        Self {
            enums_in_scope: Vec::new(),
        }
    }

    fn collect_enums(&mut self, file: &syn::File) {
        self.enums_in_scope.clear();
        for item in &file.items {
            if let Item::Enum(item_enum) = item {
                let mut variants = Vec::new();
                for variant in &item_enum.variants {
                    let arity = match &variant.fields {
                        syn::Fields::Named(_) => 0,
                        syn::Fields::Unnamed(fields) => fields.unnamed.len(),
                        syn::Fields::Unit => 0,
                    };
                    variants.push(VariantInfo {
                        name: variant.ident.clone(),
                        arity,
                    });
                }
                self.enums_in_scope.push(EnumInfo {
                    name: item_enum.ident.clone(),
                    variants,
                });
            }
        }
    }

    fn find_candidate_variant(&self, current_variant_name: &Ident, current_arity: usize) -> Option<(Ident, Ident)> {
        for enum_info in &self.enums_in_scope {
            for variant_info in &enum_info.variants {
                if variant_info.name == *current_variant_name && variant_info.arity != current_arity {
                    return Some((enum_info.name.clone(), variant_info.name.clone()));
                }
            }
        }
        None
    }
}

impl VisitMut for MatchArmVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        let mut items = Vec::new();
        for attr in &expr.attrs {
            items.push(syn::Item::Verbatim(attr.to_token_stream()));
        }
        self.collect_enums(&syn::File {
            shebang: None,
            attrs: Vec::new(),
            items,
        });

        for arm in &mut expr.arms {
            if let Pat::Path(pat_path) = &mut arm.pat {
                if let Some(path_segment) = pat_path.path.segments.last() {
                    let variant_name = &path_segment.ident;
                    let current_arity = match &path_segment.arguments {
                        PathArguments::Parenthesized(args) => args.inputs.len(),
                        _ => 0,
                    };

                    if let Some((enum_name, _)) = self.find_candidate_variant(variant_name, current_arity) {
                        let new_path = syn::Path {
                            leading_colon: Some(token::Colon::default()),
                            segments: {
                                let mut segments = Punctuated::new();
                                segments.push(syn::PathSegment {
                                    ident: enum_name,
                                    arguments: PathArguments::None,
                                });
                                segments.push(syn::PathSegment {
                                    ident: variant_name.clone(),
                                    arguments: path_segment.arguments.clone(),
                                });
                                segments
                            },
                        };
                        arm.pat = Pat::Path(syn::PatPath {
                            attrs: pat_path.attrs.clone(),
                            qself: None,
                            path: new_path,
                        });
                    }
                }
            } else if let Pat::TupleStruct(pat_tuple) = &mut arm.pat {
                let variant_name = &pat_tuple.path.segments.last().unwrap().ident;
                let current_arity = pat_tuple.elems.len();

                if let Some((enum_name, _)) = self.find_candidate_variant(variant_name, current_arity) {
                    let new_path = syn::Path {
                        leading_colon: Some(token::Colon::default()),
                        segments: {
                            let mut segments = Punctuated::new();
                            segments.push(syn::PathSegment {
                                ident: enum_name,
                                arguments: PathArguments::None,
                            });
                            segments.push(syn::PathSegment {
                                ident: variant_name.clone(),
                                arguments: PathArguments::None,
                            });
                            segments
                        },
                    };
                    arm.pat = Pat::TupleStruct(syn::PatTupleStruct {
                        attrs: pat_tuple.attrs.clone(),
                        qself: None,
                        path: new_path,
                        paren_token: pat_tuple.paren_token,
                        elems: pat_tuple.elems.clone(),
                    });
                }
            }
        }
        visit_mut::visit_expr_match_mut(self, expr);
    }
}