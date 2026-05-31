use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprLit, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lit, Local, Pat, PatType, Path as SynPath,
    PathArguments, AngleBracketedGenericArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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
                    let new_const_param = parse_quote!(const OTHER_CONST: usize = 0);
                    new_params.push(new_const_param);
                    func.sig.generics.params = new_params;
                    const_params_found.push((target_param.clone(), Ident::new("OTHER_CONST", Span::call_site())));
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
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
                            let new_const_param = parse_quote!(const OTHER_CONST: usize = 0);
                            new_params.push(new_const_param);
                            func.sig.generics.params = new_params;
                            const_params_found.push((target_param.clone(), Ident::new("OTHER_CONST", Span::call_site())));
                        }
                    }
                }
            }
        }
        if !const_params_found.is_empty() {
            let (target_param, other_param) = const_params_found.choose(&mut rng).unwrap().clone();
            let mut visitor = ConstGenericVisitor {
                target_param,
                other_param,
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
    target_param: Ident,
    other_param: Ident,
    rng: &'a mut R,
}

impl<'a, R: Rng> VisitMut for ConstGenericVisitor<'a, R> {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &mut *node.func {
            if let Some(last_segment) = path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    let mut new_args = args.args.clone();
                    let mut additional_args = Vec::new();
                    for (i, arg) in new_args.iter_mut().enumerate() {
                        if let GenericArgument::Const(const_arg) = arg {
                            if let Expr::Lit(ExprLit { lit: Lit::Int(int_lit), .. }) = const_arg {
                                let value = int_lit.base10_parse::<usize>().unwrap_or(0);
                                let other_param = self.other_param.clone();
                                let new_expr: Expr = parse_quote!({ #other_param + (#value - #other_param) });
                                *const_arg = new_expr;
                                if self.rng.gen_bool(0.5) {
                                    let additional_arg = GenericArgument::Const(parse_quote!(#value));
                                    additional_args.push(additional_arg);
                                }
                            }
                        }
                    }
                    new_args.extend(additional_args);
                    last_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                        colon2_token: args.colon2_token,
                        lt_token: args.lt_token,
                        args: new_args,
                        gt_token: args.gt_token,
                    });
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
}