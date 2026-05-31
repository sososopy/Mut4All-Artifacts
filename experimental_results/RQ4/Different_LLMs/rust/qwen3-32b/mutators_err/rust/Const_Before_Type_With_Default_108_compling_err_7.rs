use crate::mutator::Mutator;
use syn::parse_quote;
use syn::punctuated::Punctuated;
use syn::GenericParam;
use syn::token::Comma;

pub struct Const_Before_Type_With_Default_108;

impl Mutator for Const_Before_Type_With_Default_108 {
    fn name(&self) -> &str {
        "Const_Before_Type_With_Default_108"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                process_generics(&mut s.generics.params);
            } else if let syn::Item::Enum(e) = item {
                process_generics(&mut e.generics.params);
            } else if let syn::Item::Trait(t) = item {
                process_generics(&mut t.generics.params);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn process_generics(params: &mut Punctuated<GenericParam, Comma>) {
    let has_const = params.iter().any(|p| matches!(p, GenericParam::Const(_)));
    let has_type = params.iter().any(|p| matches!(p, GenericParam::Type(_)));
    if !(has_const && has_type) {
        return;
    }

    if let Some(idx) = params.iter().position(|p| matches!(p, GenericParam::Const(_))) {
        let const_param = params.drain(idx..idx+1).next().unwrap();
        if let GenericParam::Const(mut cp) = const_param {
            if cp.default.is_none() {
                cp.default = Some(parse_quote!({0}));
            }
            params.insert(0, GenericParam::Const(cp));
        }
    }
}