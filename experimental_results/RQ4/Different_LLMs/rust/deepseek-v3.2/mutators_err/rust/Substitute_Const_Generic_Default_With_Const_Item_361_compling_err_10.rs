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

pub struct Substitute_Const_Generic_Default_With_Const_Item_361;

impl Mutator for Substitute_Const_Generic_Default_With_Const_Item_361 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Default_With_Const_Item_361"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_items = Vec::new();
        let mut const_generic_targets = Vec::new();

        struct ConstCollector<'a> {
            items: &'a mut Vec<(Ident, Type)>,
        }
        impl<'ast> Visit<'ast> for ConstCollector<'ast> {
            fn visit_item_const(&mut self, node: &'ast syn::ItemConst) {
                self.items.push((node.ident.clone(), *node.ty.clone()));
                syn::visit::visit_item_const(self, node);
            }
        }

        struct GenericParamCollector<'ast, 'a> {
            targets: &'a mut Vec<(GenericParam, Option<&'ast syn::Item>)>,
            current_item: Option<&'ast syn::Item>,
        }
        impl<'ast> Visit<'ast> for GenericParamCollector<'ast, '_> {
            fn visit_item(&mut self, node: &'ast syn::Item) {
                let prev_item = self.current_item.replace(node);
                syn::visit::visit_item(self, node);
                self.current_item = prev_item;
            }
            fn visit_generic_param(&mut self, node: &'ast syn::GenericParam) {
                if let GenericParam::Const(const_param) = node {
                    if const_param.default.is_some() {
                        self.targets.push((node.clone(), self.current_item));
                    }
                }
                syn::visit::visit_generic_param(self, node);
            }
        }

        {
            let mut collector = ConstCollector { items: &mut const_items };
            collector.visit_file(file);
        }
        {
            let mut collector = GenericParamCollector { targets: &mut const_generic_targets, current_item: None };
            collector.visit_file(file);
        }

        let mut rng = thread_rng();
        for (generic_param, parent_item) in const_generic_targets {
            if let GenericParam::Const(const_param) = generic_param {
                if let Some(default_expr) = &const_param.default {
                    let param_type = const_param.ty.clone();
                    let mut candidate_consts = const_items.iter()
                        .filter(|(_, ty)| {
                            let param_type_str = quote!(#param_type).to_string();
                            let ty_str = quote!(#ty).to_string();
                            param_type_str == ty_str
                        })
                        .map(|(ident, _)| ident.clone())
                        .collect::<Vec<_>>();

                    if candidate_consts.is_empty() {
                        let new_const_name = Ident::new(&format!("ALTERNATE_{}", rng.gen_range(0..10000)), Span::call_site());
                        let new_const_item: syn::ItemConst = parse_quote! {
                            const #new_const_name: #param_type = 0;
                        };
                        let module_items = &mut file.items;
                        module_items.insert(0, syn::Item::Const(new_const_item.clone()));
                        const_items.push((new_const_name.clone(), *param_type.clone()));
                        candidate_consts.push(new_const_name);
                    }

                    if let Some(selected_const) = candidate_consts.choose(&mut rng) {
                        let new_default: syn::Expr = parse_quote!(#selected_const);
                        if let Some(parent_item) = parent_item {
                            match parent_item {
                                syn::Item::Struct(item_struct) => {
                                    let mut generics = item_struct.generics.clone();
                                    for param in &mut generics.params {
                                        if let GenericParam::Const(cp) = param {
                                            if cp.ident == const_param.ident {
                                                cp.default = Some(new_default.clone());
                                            }
                                        }
                                    }
                                    if let syn::Item::Struct(item) = parent_item {
                                        let mut new_item = item_struct.clone();
                                        new_item.generics = generics;
                                        for module_item in &mut file.items {
                                            if let syn::Item::Struct(existing_item) = module_item {
                                                if existing_item.ident == item.ident {
                                                    *existing_item = new_item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                syn::Item::Enum(item_enum) => {
                                    let mut generics = item_enum.generics.clone();
                                    for param in &mut generics.params {
                                        if let GenericParam::Const(cp) = param {
                                            if cp.ident == const_param.ident {
                                                cp.default = Some(new_default.clone());
                                            }
                                        }
                                    }
                                    if let syn::Item::Enum(item) = parent_item {
                                        let mut new_item = item_enum.clone();
                                        new_item.generics = generics;
                                        for module_item in &mut file.items {
                                            if let syn::Item::Enum(existing_item) = module_item {
                                                if existing_item.ident == item.ident {
                                                    *existing_item = new_item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                syn::Item::Union(item_union) => {
                                    let mut generics = item_union.generics.clone();
                                    for param in &mut generics.params {
                                        if let GenericParam::Const(cp) = param {
                                            if cp.ident == const_param.ident {
                                                cp.default = Some(new_default.clone());
                                            }
                                        }
                                    }
                                    if let syn::Item::Union(item) = parent_item {
                                        let mut new_item = item_union.clone();
                                        new_item.generics = generics;
                                        for module_item in &mut file.items {
                                            if let syn::Item::Union(existing_item) = module_item {
                                                if existing_item.ident == item.ident {
                                                    *existing_item = new_item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                syn::Item::Trait(item_trait) => {
                                    let mut generics = item_trait.generics.clone();
                                    for param in &mut generics.params {
                                        if let GenericParam::Const(cp) = param {
                                            if cp.ident == const_param.ident {
                                                cp.default = Some(new_default.clone());
                                            }
                                        }
                                    }
                                    if let syn::Item::Trait(item) = parent_item {
                                        let mut new_item = item_trait.clone();
                                        new_item.generics = generics;
                                        for module_item in &mut file.items {
                                            if let syn::Item::Trait(existing_item) = module_item {
                                                if existing_item.ident == item.ident {
                                                    *existing_item = new_item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                syn::Item::Fn(item_fn) => {
                                    let mut generics = item_fn.sig.generics.clone();
                                    for param in &mut generics.params {
                                        if let GenericParam::Const(cp) = param {
                                            if cp.ident == const_param.ident {
                                                cp.default = Some(new_default.clone());
                                            }
                                        }
                                    }
                                    if let syn::Item::Fn(item) = parent_item {
                                        let mut new_item = item_fn.clone();
                                        new_item.sig.generics = generics;
                                        for module_item in &mut file.items {
                                            if let syn::Item::Fn(existing_item) = module_item {
                                                if existing_item.sig.ident == item.sig.ident {
                                                    *existing_item = new_item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                syn::Item::Impl(item_impl) => {
                                    let mut generics = item_impl.generics.clone();
                                    for param in &mut generics.params {
                                        if let GenericParam::Const(cp) = param {
                                            if cp.ident == const_param.ident {
                                                cp.default = Some(new_default.clone());
                                            }
                                        }
                                    }
                                    if let syn::Item::Impl(item) = parent_item {
                                        let mut new_item = item_impl.clone();
                                        new_item.generics = generics;
                                        for module_item in &mut file.items {
                                            if let syn::Item::Impl(existing_item) = module_item {
                                                if existing_item.self_ty == item.self_ty {
                                                    *existing_item = new_item;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes default expressions of const generic parameters with existing const items of matching type, or introduces new const items when none exist. This transformation creates complex substitution scenarios, potential dependency cycles, and out-of-range errors during monomorphization. It targets struct, enum, union, trait, function, and impl definitions to stress the compiler's const generic default handling."
    }
}