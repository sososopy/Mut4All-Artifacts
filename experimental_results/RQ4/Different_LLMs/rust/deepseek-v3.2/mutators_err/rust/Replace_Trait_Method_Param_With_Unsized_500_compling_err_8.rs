use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, TypeSlice, TypeTraitObject, parse_quote,
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

pub struct Replace_Trait_Method_Param_With_Unsized_500;

impl Mutator for Replace_Trait_Method_Param_With_Unsized_500 {
    fn name(&self) -> &str {
        "Replace_Trait_Method_Param_With_Unsized_500"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_traits = HashSet::new();
        let mut existing_slice_types = HashSet::new();
        let mut existing_str_like = false;
        let mut visitor = CollectTypesVisitor {
            traits: &mut existing_traits,
            slices: &mut existing_slice_types,
            has_str: &mut existing_str_like,
        };
        visitor.visit_file(file);
        let mut unsized_candidates = Vec::new();
        for trait_name in &existing_traits {
            unsized_candidates.push(Type::TraitObject(TypeTraitObject {
                dyn_token: Some(token::Dyn::default()),
                bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: SynPath::from(Ident::new(trait_name, Span::call_site())),
                })]),
            }));
        }
        for slice_elem in &existing_slice_types {
            unsized_candidates.push(Type::Slice(TypeSlice {
                bracket_token: token::Bracket::default(),
                elem: Box::new(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(Ident::new(slice_elem, Span::call_site())),
                })),
            }));
        }
        if existing_str_like {
            unsized_candidates.push(Type::Path(TypePath {
                qself: None,
                path: SynPath::from(Ident::new("str", Span::call_site())),
            }));
        }
        let mut rng = thread_rng();
        let mut items_to_insert = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let syn::TraitItem::Fn(trait_method) = trait_item {
                        if trait_method.default.is_some() {
                            if let Some(abi) = &trait_method.sig.abi {
                                if abi.name.as_ref().map_or(false, |lit| lit.value() == "C") {
                                    for input in &mut trait_method.sig.inputs {
                                        if let FnArg::Typed(pat_type) = input {
                                            if is_sized_type(&pat_type.ty) {
                                                let unsized_type = if !unsized_candidates.is_empty() {
                                                    unsized_candidates.choose(&mut rng).unwrap().clone()
                                                } else {
                                                    let dummy_trait = Item::Trait(ItemTrait {
                                                        attrs: Vec::new(),
                                                        vis: syn::Visibility::Public(Default::default()),
                                                        unsafety: None,
                                                        auto_token: None,
                                                        trait_token: token::Trait::default(),
                                                        ident: Ident::new("DummyTrait", Span::call_site()),
                                                        generics: Generics::default(),
                                                        colon_token: None,
                                                        supertraits: Punctuated::new(),
                                                        brace_token: token::Brace::default(),
                                                        items: Vec::new(),
                                                        restriction: Default::default(),
                                                    });
                                                    items_to_insert.push(dummy_trait);
                                                    Type::TraitObject(TypeTraitObject {
                                                        dyn_token: Some(token::Dyn::default()),
                                                        bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                                                            paren_token: None,
                                                            modifier: TraitBoundModifier::None,
                                                            lifetimes: None,
                                                            path: SynPath::from(Ident::new("DummyTrait", Span::call_site())),
                                                        })]),
                                                    })
                                                };
                                                pat_type.ty = Box::new(unsized_type);
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
        for item in items_to_insert {
            file.items.insert(0, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait methods with extern \"C\" ABI and default implementations. It replaces sized parameter types with unsized types (dyn Trait, [T], or str) to trigger improper C type checking. If no suitable unsized type exists in scope, it introduces a new dummy trait. This transformation creates type mismatches between the signature and body, testing the compiler's handling of unsized types in foreign function interfaces and its linting passes."
    }
}

struct CollectTypesVisitor<'a> {
    traits: &'a mut HashSet<String>,
    slices: &'a mut HashSet<String>,
    has_str: &'a mut bool,
}

impl<'a> Visit<'a> for CollectTypesVisitor<'a> {
    fn visit_item_trait(&mut self, node: &'a ItemTrait) {
        self.traits.insert(node.ident.to_string());
        syn::visit::visit_item_trait(self, node);
    }
    fn visit_type_slice(&mut self, node: &'a TypeSlice) {
        if let Type::Path(type_path) = &*node.elem {
            if let Some(segment) = type_path.path.segments.last() {
                self.slices.insert(segment.ident.to_string());
            }
        }
        syn::visit::visit_type_slice(self, node);
    }
    fn visit_type_path(&mut self, node: &'a TypePath) {
        if node.path.is_ident("str") {
            *self.has_str = true;
        }
        syn::visit::visit_type_path(self, node);
    }
}

fn is_sized_type(ty: &Type) -> bool {
    match ty {
        Type::Array(_) | Type::Slice(_) | Type::TraitObject(_) => false,
        Type::Path(type_path) => {
            if type_path.path.is_ident("str") {
                false
            } else {
                true
            }
        }
        _ => true,
    }
}