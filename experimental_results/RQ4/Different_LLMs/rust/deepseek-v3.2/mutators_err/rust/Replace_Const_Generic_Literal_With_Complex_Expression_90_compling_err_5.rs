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

pub struct Replace_Const_Generic_Literal_With_Complex_Expression_90;

impl Mutator for Replace_Const_Generic_Literal_With_Complex_Expression_90 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Literal_With_Complex_Expression_90"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut const_params_found = Vec::new();
        let mut target_params = Vec::new();
        let mut other_params = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut const_params: Vec<(Ident, usize)> = Vec::new();
                for param in &func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                            if path.segments.last().map(|seg| seg.ident == "usize").unwrap_or(false) {
                                const_params.push((const_param.ident.clone(), 0));
                            }
                        }
                    }
                }
                if const_params.len() > 0 {
                    let target_param_idx = rng.gen_range(0..const_params.len());
                    let target_param = const_params[target_param_idx].0.clone();
                    let mut new_params = func.sig.generics.params.clone();
                    let new_const_param = parse_2quote!(const OTHER_CONST: usize = 0);
                    new_params.push(new_const_param);
                    func.sig.generics.params = new_params;
                    const_params_found.push(true);
                    target_params.push(target_param);
                    other_params.push(Ident::new("OTHER_CONST", Span::call_site()));
                } else {
                    const_params_found.push(false);
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        let mut const_params: Vec<(Ident, usize)> = Vec::new();
                        for param in &func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Type::Path(TypePath { path, .. }) = &const_param.ty {
                                    if path.segments.last().map(|seg| seg.ident == "usize").unwrap_or(false) {
                                        const_params.push((const_param.ident.clone(), 0));
                                    }
                                }
                            }
                        }
                        if const_params.len() > 0 {
                            let target_param_idx = rng.gen_range(0..const_params.len());
                            let target_param = const_params[target_param_idx].0.clone();
                            let mut new_params = func.sig.generics.params.clone();
                            let new_const_param = parse_2quote!(const OTHER_CONST: usize = 0);
                            new_params.push(new_const_param);
                            func.sig.generics.params = new_params;
                            const_params_found.push(true);
                            target_params.push(target_param);
                            other_params.push(Ident::new("OTHER_CONST", Span::call_site()));
                        } else {
                            const_params_found.push(false);
                        }
                    }
                }
            }
        }
        if const_params_found.iter().any(|&found| found) {
            let mut visitor = ConstGenericVisitor {
                target_params,
                other_params,
                rng: &mut rng,
            };
            visitor.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments in function calls where the argument is an integer literal. It replaces the literal with a complex expression that depends on another const generic parameter, introducing polymorphic constants. If no suitable parameter exists, it adds a new const generic parameter with a default value. This transformation stresses the compiler's const evaluation and generic instantiation phases, potentially triggering bugs related to UnevaluatedConst during monomorphization."
    }
}

struct ConstGenericVisitor<'a, R: Rng> {
    target_params: Vec<Ident>,
    other_params: Vec<Ident>,
    rng: &'a mut R,
}

impl<'a, R: Rng> VisitMut for ConstGenericVisitor<'a, R> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if let Some(last_segment) = path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                    let mut new_args = args.args.clone();
                    let mut additional_args = Vec::new();
                    for (i, arg) in new_args.iter_mut().enumerate() {
                        if let GenericArgument::Const(const_arg) = arg {
                            if let Expr::Lit(ExprLit { lit: Lit::Int(int_lit), .. }) = const_arg {
                                let value = int_lit.base10_parse::<usize>().unwrap_or(0);
                                if self.target_params.len() > 0 && self.other_params.len() > 0 {
                                    let target_idx = self.rng.gen_range(0..self.target_params.len());
                                    let target_param = &self.target_params[target_idx];
                                    let other_param = &self.other_params[target_idx];
                                    let new_expr: Expr = parse_quote!({ #other_param + (#value - #other_param) });
                                    *const_arg = new_expr;
                                    if self.rng.gen_bool(0.5) {
                                        let additional_arg = GenericArgument::Const(parse_quote!(#value));
                                        additional_args.push(additional_arg);
                                    }
                                }
                            }
                        }
                    }
                    new_args.extend(additional_args);
                    let mut path_mut = node.func.as_mut();
                    if let Expr::Path(ExprPath { path: path_mut_ref, .. }) = path_mut {
                        let mut last_segment_mut = path_mut_ref.segments.last_mut().unwrap();
                        last_segment_mut.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: args.colon2_token,
                            lt_token: args.lt_token,
                            args: new_args,
                            gt_token: args.gt_token,
                        });
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}