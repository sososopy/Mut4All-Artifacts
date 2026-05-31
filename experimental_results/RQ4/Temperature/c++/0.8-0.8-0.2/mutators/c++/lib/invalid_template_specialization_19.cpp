//source file
#include "../include/invalid_template_specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateStruct")) {
        //Filter nodes in header files
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                        TS->getLocation()))
            return;
        
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedSpecialization = "\ntemplate<> struct " + TS->getNameAsString() + "<UndefinedType> { int data; }; /*mut19*/";
        
        //Replace the original AST node with the mutated one
        declaration += mutatedSpecialization;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}