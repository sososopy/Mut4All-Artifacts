//source file
#include "../include/replace_template_nested_type_reference_558.h"

// ========================================================================================================
#define MUT558_OUTPUT 1

void MutatorFrontendAction_558::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation()) {
            //Get the source code text of target node
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string mutatedBody = functionBody;
            size_t pos = mutatedBody.find("&S<T2>::S<int>");
            if (pos != std::string::npos) {
                mutatedBody.replace(pos, std::string("&S<T2>::S<int>").length(), "&AnotherStruct<U>::AnotherTemplateFunction<float>");
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), mutatedBody);
        }
    }
}
  
void MutatorFrontendAction_558::MutatorASTConsumer_558::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}