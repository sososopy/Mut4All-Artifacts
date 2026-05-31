//source file
#include "../include/misplace_template_arg_in_struct_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("StaticFunc")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;
        
        // Get the return type
        QualType returnType = MT->getReturnType();
        if (const TemplateSpecializationType *TST = returnType->getAs<TemplateSpecializationType>()) {
            // Get the source code text of target node
            SourceRange range = MT->getReturnTypeSourceRange();
            std::string replacement = Rewrite.getRewrittenText(range);

            // Perform mutation on the source code text by applying string replacement
            if (replacement.find(">>") == std::string::npos) {
                replacement.insert(replacement.find(">"), ">");
            } else {
                replacement.erase(replacement.find_last_of(">"));
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(range, replacement);
        }
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isStaticStorageClass(), returns(templateSpecializationType())).bind("StaticFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}