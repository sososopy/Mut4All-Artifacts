use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemType, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Trait_Method_Signature_Type_389;

impl Mutator for Replace_Trait_Method_Signature_Type_389 {
    fn name(&self) -> &str {
        "Replace_Trait_Method_Signature_Type_389"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_collector = TypeCollector::new();
        type_collector.visit_file(file);
        let available_types = type_collector.types;
        if available_types.is_empty() {
            return;
        }

        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let mut rng = thread_rng();
                        let mut changed = false;
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if rng.gen_bool(0.5) {
                                    if let Some(new_type) = available_types.choose(&mut rng) {
                                        pat_type.ty = Box::new(new_type.clone());
                                        changed = true;
                                    }
                                }
                            }
                        }
                        if !changed {
                            if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                                if rng.gen_bool(0.5) {
                                    if let Some(new_type) = available_types.choose(&mut rng) {
                                        *return_type = Box::new(new_type.clone());
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
        "The mutation operator targets trait method signatures within impl blocks. It randomly replaces parameter types or return types with other concrete types already present in the seed program. This creates type mismatches between the trait definition and its implementation, potentially triggering bugs in trait resolution, const evaluation, and type layout reconciliation, especially under features like generic_const_exprs."
    }
}

struct TypeCollector {
    types: Vec<Type>,
}

impl TypeCollector {
    fn new() -> Self {
        Self { types: Vec::new() }
    }
}

impl Visit<'_> for TypeCollector {
    fn visit_type(&mut self, node: &Type) {
        match node {
            Type::Path(type_path) => {
                if !type_path.path.segments.is_empty() {
                    self.types.push(node.clone());
                }
            }
            Type::Slice(_) => self.types.push(node.clone()),
            Type::Array(_) => self.types.push(node.clone()),
            Type::Ptr(_) => self.types.push(node.clone()),
            Type::Reference(_) => self.types.push(node.clone()),
            Type::Tuple(_) => self.types.push(node.clone()),
            Type::Never(_) => self.types.push(node.clone()),
            Type::Infer(_) => {}
            Type::Macro(_) => {}
            Type::ImplTrait(_) => {}
            Type::TraitObject(_) => {}
            Type::Group(_) => {}
            Type::Paren(_) => {}
            Type::Verbatim(_) => {}
            Type::BareFn(_) => self.types.push(node.clone()),
            _ => {}
        }
        syn::visit::visit_type(self, node);
    }

    fn visit_item_struct(&mut self, node: &ItemStruct) {
        self.types.push(Type::Path(TypePath {
            qself: None,
            path: SynPath::from(node.ident.clone()),
        }));
        syn::visit::visit_item_struct(self, node);
    }

    fn visit_item_enum(&mut self, node: &ItemEnum) {
        self.types.push(Type::Path(TypePath {
            qself: None,
            path: SynPath::from(node.ident.clone()),
        }));
        syn::visit::visit_item_enum(self, node);
    }

    fn visit_item_type(&mut self, node: &ItemType) {
        self.types.push(Type::Path(TypePath {
            qself: None,
            path: SynPath::from(node.ident.clone()),
        }));
        syn::visit::visit_item_type(self, node);
    }
}