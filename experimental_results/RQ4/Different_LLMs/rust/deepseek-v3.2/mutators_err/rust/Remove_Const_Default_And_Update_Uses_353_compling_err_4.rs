use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, PathSegment, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Remove_Const_Default_And_Update_Uses_353;

impl Mutator for Remove_Const_Default_And_Update_Uses_353 {
    fn name(&self) -> &str {
        "Remove_Const_Default_And_Update_Uses_353"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_info = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_params_to_remove = Vec::new();
                let generics_clone = item_struct.generics.clone();
                for (idx, param) in item_struct.generics.params.iter_mut().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            let param_name = const_param.ident.clone();
                            let default_expr = const_param.default.take().unwrap();
                            struct_info.push((item_struct.ident.clone(), param_name, default_expr, generics_clone.clone()));
                            const_params_to_remove.push(idx);
                        }
                    }
                }
                let mut new_params = Punctuated::<GenericParam, Comma>::new();
                for (idx, param) in item_struct.generics.params.iter().enumerate() {
                    if !const_params_to_remove.contains(&idx) {
                        new_params.push(param.clone());
                    }
                }
                item_struct.generics.params = new_params;
            }
        }
        for (struct_name, param_name, default_expr, generics) in struct_info {
            let mut visitor = StructUsageVisitor {
                struct_name: struct_name.clone(),
                param_name: param_name.clone(),
                default_expr: default_expr.clone(),
                generics: generics.clone(),
            };
            visitor.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with generic const parameters that have default values and where clauses that depend on those const parameters. It removes the default value assignment from the const parameter in the struct definition. For each usage of the struct type without explicit const arguments, it replaces them with an instantiation using the original default expression. This transformation forces explicit instantiation at use sites, altering the substitution environment and potentially exposing ICEs in the specialization graph construction or type folding logic."
    }
}

struct StructUsageVisitor {
    struct_name: Ident,
    param_name: Ident,
    default_expr: Expr,
    generics: Generics,
}

impl VisitMut for StructUsageVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == self.struct_name {
                let segment = &mut type_path.path.segments[0];
                if let PathArguments::None = segment.arguments {
                    let mut args = Punctuated::<GenericArgument, Comma>::new();
                    for param in &self.generics.params {
                        match param {
                            GenericParam::Type(type_param) => {
                                args.push(GenericArgument::Type(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(type_param.ident.clone()),
                                })));
                            }
                            GenericParam::Lifetime(lifetime_param) => {
                                args.push(GenericArgument::Lifetime(lifetime_param.lifetime.clone()));
                            }
                            GenericParam::Const(const_param) => {
                                if const_param.ident == self.param_name {
                                    let default_expr = self.default_expr.clone();
                                    args.push(GenericArgument::Const(Expr::Verbatim(quote!(#default_expr).into())));
                                } else {
                                    let default_expr = self.default_expr.clone();
                                    args.push(GenericArgument::Const(Expr::Verbatim(quote!(#default_expr).into())));
                                }
                            }
                        }
                    }
                    segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                        colon2_token: None,
                        lt_token: token::Lt::default(),
                        args,
                        gt_token: token::Gt::default(),
                    });
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        if let Type::Path(type_path) = &*i.self_ty {
            if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == self.struct_name {
                let segment = &type_path.path.segments[0];
                if let PathArguments::None = segment.arguments {
                    let mut args = Punctuated::<GenericArgument, Comma>::new();
                    for param in &self.generics.params {
                        match param {
                            GenericParam::Type(type_param) => {
                                args.push(GenericArgument::Type(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(type_param.ident.clone()),
                                })));
                            }
                            GenericParam::Lifetime(lifetime_param) => {
                                args.push(GenericArgument::Lifetime(lifetime_param.lifetime.clone()));
                            }
                            GenericParam::Const(const_param) => {
                                if const_param.ident == self.param_name {
                                    let default_expr = self.default_expr.clone();
                                    args.push(GenericArgument::Const(Expr::Verbatim(quote!(#default_expr).into())));
                                } else {
                                    let default_expr = self.default_expr.clone();
                                    args.push(GenericArgument::Const(Expr::Verbatim(quote!(#default_expr).into())));
                                }
                            }
                        }
                    }
                    i.self_ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: {
                                let mut segs = Punctuated::new();
                                segs.push(PathSegment {
                                    ident: self.struct_name.clone(),
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: token::Lt::default(),
                                        args,
                                        gt_token: token::Gt::default(),
                                    }),
                                });
                                segs
                            },
                        },
                    }));
                }
            }
        }
        syn::visit_mut::visit_item_impl_mut(self, i);
    }
}