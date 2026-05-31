use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, thread_rng};
use std::collections::HashMap;
use syn::{
    Expr, ExprPath, File, GenericArgument, Item, ItemConst, PathArguments, Type, TypePath,
    parse_quote, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Replace_Const_Param_With_Unevaluated_390;

impl Mutator for Replace_Const_Param_With_Unevaluated_390 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Unevaluated_390"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_name_to_unevaluated = HashMap::new();
        let mut unevaluated_consts = Vec::new();
        
        for item in &file.items {
            if let Item::Const(item_const) = item {
                let is_unevaluated = match &*item_const.expr {
                    Expr::Lit(_) => false,
                    _ => true,
                };
                let name = item_const.ident.to_string();
                const_name_to_unevaluated.insert(name, is_unevaluated);
                if is_unevaluated {
                    unevaluated_consts.push(item_const.ident.to_string());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    for segment in &mut type_path.path.segments {
                        if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Expr(expr) = arg {
                                    if let Expr::Path(expr_path) = &**expr {
                                        let path_str = expr_path.path.segments.last().unwrap().ident.to_string();
                                        if let Some(&is_concrete) = const_name_to_unevaluated.get(&path_str) {
                                            if is_concrete && !unevaluated_consts.is_empty() {
                                                let selected_name = unevaluated_consts[thread_rng().gen_range(0..unevaluated_consts.len())].clone();
                                                let new_expr = parse_quote!(#selected_name);
                                                *expr = Box::new(new_expr);
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
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}