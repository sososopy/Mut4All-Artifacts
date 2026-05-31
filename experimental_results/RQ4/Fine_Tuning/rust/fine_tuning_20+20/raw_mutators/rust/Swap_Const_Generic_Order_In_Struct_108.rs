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

pub struct Swap_Const_Generic_Order_In_Struct_108;

impl Mutator for Swap_Const_Generic_Order_In_Struct_108 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Order_In_Struct_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_names = HashSet::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                struct_names.insert(item_struct.ident.to_string());
            }
        }
        let mut const_generic_structs = Vec::new();
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Struct(item_struct) = item {
                let mut has_default_const = false;
                let mut has_other_param = false;
                for param in &item_struct.generics.params {
                    match param {
                        GenericParam::Type(type_param) => {
                            if type_param.default.is_some() {
                                has_other_param = true;
                            }
                        }
                        GenericParam::Const(const_param) => {
                            if const_param.default.is_some() {
                                has_default_const = true;
                            } else {
                                has_other_param = true;
                            }
                        }
                        _ => {}
                    }
                }
                if has_default_const && has_other_param {
                    const_generic_structs.push(index);
                }
            }
        }
        if const_generic_structs.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let chosen_index = const_generic_structs.choose(&mut rng).unwrap();
        if let Item::Struct(item_struct) = &mut file.items[*chosen_index] {
            let mut const_params = Vec::new();
            let mut other_params = Vec::new();
            for param in &item_struct.generics.params {
                match param {
                    GenericParam::Const(const_param) => {
                        if const_param.default.is_some() {
                            const_params.push(param.clone());
                        } else {
                            other_params.push(param.clone());
                        }
                    }
                    _ => {
                        other_params.push(param.clone());
                    }
                }
            }
            if const_params.is_empty() {
                return;
            }
            let mut new_params = Punctuated::new();
            for param in other_params {
                new_params.push(param);
            }
            for param in const_params {
                new_params.push(param);
            }
            item_struct.generics.params = new_params;
        }
        let mut visitor = StructUsageVisitor {
            struct_names,
            usage_spans: Vec::new(),
        };
        visitor.visit_file(file);
        let mut usage_spans = visitor.usage_spans;
        usage_spans.sort_by_key(|span| span.start());
        usage_spans.dedup();
        let mut new_items = Vec::new();
        for item in file.items.drain(..) {
            new_items.push(item);
            if let Item::Struct(item_struct) = &item {
                if usage_spans.contains(&item_struct.span()) {
                    let dummy_item: Item = parse_quote! {
                        struct __Dummy;
                    };
                    new_items.push(dummy_item);
                }
            }
        }
        file.items = new_items;
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generics, specifically those with a const parameter that has a default value. It swaps the order of the const parameter with other generic parameters, ensuring the const parameter is defined after any type or non-default const parameters. This transformation exploits a known compiler bug related to const generic ordering and default expressions, aiming to trigger ICEs or miscompilations by altering the struct's generic parameter layout while preserving its usage in the code."
    }
}

struct StructUsageVisitor {
    struct_names: HashSet<String>,
    usage_spans: Vec<proc_macro2::Span>,
}

impl<'ast> Visit<'ast> for StructUsageVisitor {
    fn visit_type_path(&mut self, node: &'ast TypePath) {
        if let Some(ident) = node.path.get_ident() {
            if self.struct_names.contains(&ident.to_string()) {
                self.usage_spans.push(node.span());
            }
        }
        syn::visit::visit_type_path(self, node);
    }
    fn visit_expr_path(&mut self, node: &'ast ExprPath) {
        if let Some(ident) = node.path.get_ident() {
            if self.struct_names.contains(&ident.to_string()) {
                self.usage_spans.push(node.span());
            }
        }
        syn::visit::visit_expr_path(self, node);
    }
}