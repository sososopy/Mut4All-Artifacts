use syn::{parse_quote, Expr, Ident};
use proc_macro2::Span;
use rand::Rng;
use rand::thread_rng;
use rand::prelude::SliceRandom;

struct Replace_Static_Variable_Initialization_68;

impl Replace_Static_Variable_Initialization_68 {
    fn new() -> Self {
        Replace_Static_Variable_Initialization_68
    }
}

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Static_Variable_Initialization_68 {
    fn name(&self) -> &str {
        "Replace_Static_Variable_Initialization_68"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(static_item) = item {
                if let Some(init) = &static_item.expr {
                    let new_init = self.generate_new_init(init.as_ref());
                    static_item.expr = Some(new_init);
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the initialization of static variables with new expressions that may trigger const-eval related bugs. It aims to test the compiler's handling of const evaluation and static variable initialization."
    }
}

impl Replace_Static_Variable_Initialization_68 {
    fn generate_new_init(&self, init: &Expr) -> Box<Expr> {
        let mut rng = thread_rng();
        let choices = vec![
            self.generate_struct_init(init),
            self.generate_enum_init(init),
            self.generate_function_call_init(init),
            self.generate_literal_init(init),
        ];
        choices.choose(&mut rng).unwrap().clone()
    }

    fn generate_struct_init(&self, init: &Expr) -> Box<Expr> {
        let struct_name = Ident::new(&format!("Struct_{}", rand::random::<u32>()), Span::call_site());
        let field_names = vec![
            Ident::new(&format!("field_{}", rand::random::<u32>()), Span::call_site()),
        ];
        let field_inits: Vec<Expr> = vec![parse_quote!(0)];
        Box::new(parse_quote!(#struct_name { #(#field_names: #field_inits),* }))
    }

    fn generate_enum_init(&self, init: &Expr) -> Box<Expr> {
        let enum_name = Ident::new(&format!("Enum_{}", rand::random::<u32>()), Span::call_site());
        let variant_name = Ident::new(&format!("Variant_{}", rand::random::<u32>()), Span::call_site());
        Box::new(parse_quote!(#enum_name::#variant_name))
    }

    fn generate_function_call_init(&self, init: &Expr) -> Box<Expr> {
        let function_name = Ident::new(&format!("function_{}", rand::random::<u32>()), Span::call_site());
        Box::new(parse_quote!(#function_name()))
    }

    fn generate_literal_init(&self, init: &Expr) -> Box<Expr> {
        let literal = rand::random::<i32>();
        Box::new(parse_quote!(#literal))
    }
}