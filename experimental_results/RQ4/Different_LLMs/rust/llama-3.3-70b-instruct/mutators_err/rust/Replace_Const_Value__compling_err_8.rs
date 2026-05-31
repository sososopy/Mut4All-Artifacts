use syn::parse_quote;

struct ReplaceConstValue;

impl Mutator for ReplaceConstValue {
    fn name(&self) -> &str {
        "ReplaceConstValue"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace constant values in the AST"
    }

    fn mutate(&self, file_ast: &mut syn::File) {
        // implement mutation logic here
    }
}

fn main() {
    let mut file_ast = parse_quote! {
        // example AST
    };
    let mutator = ReplaceConstValue;
    mutator.mutate(&mut file_ast);
}