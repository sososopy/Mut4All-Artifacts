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

pub struct Replace_Trait_Projection_Const_With_Generic_358;

impl Mutator for Replace_Trait_Projection_Const_With_Generic_358 {
    fn name(&self) -> &str {
        "Replace_Trait_Projection_Const_With_Generic_358"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TraitProjectionVisitor {
            rng: thread_rng(),
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates trait projections with constant arguments in associated type bounds and replaces them with const generic parameters. It first checks for existing const generic parameters in scope; if none exist, it introduces a new const generic parameter `const N: usize` to the containing item. This transformation targets the `generic_const_exprs` feature and aims to stress the compiler's handling of const generics in trait projections, potentially causing type resolution errors, const evaluation mismatches, or ICEs when a type is expected but a const generic is provided."
    }
}

struct TraitProjectionVisitor {
    rng: rand::rngs::ThreadRng,
    mutated: bool,
}

impl VisitMut for TraitProjectionVisitor {
    fn visit_item_mut(&mut self, item: &mut Item) {
        match item {
            Item::Fn(item_fn) => {
                self.visit_generics_mut(&mut item_fn.sig.generics);
                self.visit_item_fn_mut(item_fn);
            }
            Item::Impl(item_impl) => {
                self.visit_generics_mut(&mut item_impl.generics);
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        self.visit_generics_mut(&mut func.sig.generics);
                        self.visit_block_mut(&mut func.block);
                    }
                }
            }
            Item::Struct(item_struct) => {
                self.visit_generics_mut(&mut item_struct.generics);
            }
            Item::Enum(item_enum) => {
                self.visit_generics_mut(&mut item_enum.generics);
            }
            Item::Trait(item_trait) => {
                self.visit_generics_mut(&mut item_trait.generics);
            }
            _ => {}
        }
        syn::visit_mut::visit_item_mut(self, item);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Type(Type::Path(inner_type_path)) = arg {
                            self.visit_type_mut(&mut Type::Path(inner_type_path.clone()));
                        }
                    }
                }
            }
        } else if let Type::TraitObject(trait_object) = ty {
            for bound in &mut trait_object.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    self.visit_path_mut(&mut trait_bound.path);
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_path_mut(&mut self, path: &mut SynPath) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(expr) = arg {
                        if let Expr::Lit(expr_lit) = &*expr {
                            if let Lit::Int(_) = &expr_lit.lit {
                                let mut found_const_param = false;
                                let mut const_param_name = Ident::new("N", Span::call_site());
                                if let Some(parent) = self.get_current_parent() {
                                    if let Some(existing_const) = self.find_existing_const_param(&parent) {
                                        const_param_name = existing_const;
                                        found_const_param = true;
                                    }
                                    if !found_const_param {
                                        self.add_const_param_to_parent(parent);
                                        const_param_name = Ident::new("N", Span::call_site());
                                    }
                                    *arg = GenericArgument::Const(Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(const_param_name),
                                    }));
                                    self.mutated = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }
}

impl TraitProjectionVisitor {
    fn get_current_parent(&self) -> Option<GenericParent> {
        None
    }

    fn find_existing_const_param(&self, parent: &GenericParent) -> Option<Ident> {
        match parent {
            GenericParent::ItemFn(item_fn) => {
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        return Some(const_param.ident.clone());
                    }
                }
            }
            GenericParent::ImplItemFn(func) => {
                for param in &func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        return Some(const_param.ident.clone());
                    }
                }
            }
            GenericParent::Struct(item_struct) => {
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        return Some(const_param.ident.clone());
                    }
                }
            }
            GenericParent::Enum(item_enum) => {
                for param in &item_enum.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        return Some(const_param.ident.clone());
                    }
                }
            }
            GenericParent::Trait(item_trait) => {
                for param in &item_trait.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        return Some(const_param.ident.clone());
                    }
                }
            }
        }
        None
    }

    fn add_const_param_to_parent(&self, parent: GenericParent) {
        match parent {
            GenericParent::ItemFn(mut item_fn) => {
                item_fn.sig.generics.params.push(parse_quote!(const N: usize));
            }
            GenericParent::ImplItemFn(mut func) => {
                func.sig.generics.params.push(parse_quote!(const N: usize));
            }
            GenericParent::Struct(mut item_struct) => {
                item_struct.generics.params.push(parse_quote!(const N: usize));
            }
            GenericParent::Enum(mut item_enum) => {
                item_enum.generics.params.push(parse_quote!(const N: usize));
            }
            GenericParent::Trait(mut item_trait) => {
                item_trait.generics.params.push(parse_quote!(const N: usize));
            }
        }
    }
}

enum GenericParent {
    ItemFn(ItemFn),
    ImplItemFn(ItemFn),
    Struct(ItemStruct),
    Enum(ItemEnum),
    Trait(ItemTrait),
}