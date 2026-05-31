use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, Lifetime, LifetimeParam, Local, Meta, MetaList, NestedMeta, Pat, PatType, Path as SynPath,
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
    parse::ParseStream, Result as SynResult,
};

use crate::mutator::Mutator;

pub struct Change_Repr_Packed_To_C_188;

impl Mutator for Change_Repr_Packed_To_C_188 {
    fn name(&self) -> &str {
        "Change_Repr_Packed_To_C_188"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                item_enum.attrs.iter_mut().for_each(|attr| {
                    if attr.path().is_ident("repr") {
                        if let Ok(meta_list) = parse_meta_list(attr) {
                            if meta_list.iter().any(|nested| {
                                if let NestedMeta::Meta(Meta::Path(path)) = nested {
                                    path.is_ident("packed")
                                } else {
                                    false
                                }
                            }) {
                                *attr = parse_quote!(#[repr(C)]);
                            }
                        }
                    }
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum definitions with the `#[repr(packed)]` attribute. It replaces this attribute with `#[repr(C)]`, aiming to address potential alignment issues associated with packed enums. This transformation tests the compiler's handling of enum layout and alignment, potentially revealing bugs in layout calculation and alignment enforcement."
    }
}

fn parse_meta_list(attr: &Attribute) -> SynResult<Vec<NestedMeta>> {
    attr.parse_args_with(|input: ParseStream| {
        let mut nested = Vec::new();
        while !input.is_empty() {
            nested.push(input.parse()?);
            if input.peek(token::Comma) {
                let _ = input.parse::<token::Comma>();
            }
        }
        Ok(nested)
    })
}