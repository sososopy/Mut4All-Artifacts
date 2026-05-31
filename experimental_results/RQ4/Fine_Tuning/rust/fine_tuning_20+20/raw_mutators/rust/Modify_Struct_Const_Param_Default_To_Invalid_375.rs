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

pub struct Modify_Struct_Const_Param_Default_To_Invalid_375;

impl Mutator for Modify_Struct_Const_Param_Default_To_Invalid_375 {
    fn name(&self) -> &str {
        "Modify_Struct_Const_Param_Default_To_Invalid_375"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_params = Vec::new();
                for (i, param) in item_struct.generics.params.iter().enumerate() {
                    if let GenericParam::Const(_) = param {
                        const_params.push(i);
                    }
                }
                if const_params.is_empty() {
                    return;
                }
                let mut rng = thread_rng();
                let selected_index = const_params.choose(&mut rng).unwrap();
                if let GenericParam::Const(const_param) =
                    &mut item_struct.generics.params[*selected_index]
                {
                    let mut default_exprs = Vec::new();
                    if let Some(default) = &const_param.default {
                        default_exprs.push(default.clone());
                    }
                    if default_exprs.is_empty() {
                        return;
                    }
                    let selected_expr = default_exprs.choose(&mut rng).unwrap();
                    let invalid_expr: Expr = parse_quote! { "invalid" };
                    let target_span = selected_expr.span();
                    let invalid_expr = invalid_expr.into_token_stream().into_iter().map(|token| {
                        proc_macro2::TokenTree::from(proc_macro2::TokenNode::Literal(
                            proc_macro2::Literal::string("invalid"),
                        ))
                        .into()
                    });
                    let invalid_expr = proc_macro2::TokenStream::from_iter(invalid_expr);
                    let invalid_expr = syn::parse2::<Expr>(invalid_expr).unwrap();
                    let invalid_expr = invalid_expr.fold_with(&mut syn::fold::Fold::new(|node| {
                        node.replace_span(target_span)
                    }));
                    const_param.default = Some(invalid_expr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const parameters that have default values. It selects one such const parameter and replaces its default value with an invalid expression, such as a string literal, that does not conform to the expected type. This mutation is designed to exploit potential weaknesses in the compiler's handling of const generics by introducing type mismatches in default values, which can trigger errors or unexpected behavior during type checking and const evaluation."
    }
}