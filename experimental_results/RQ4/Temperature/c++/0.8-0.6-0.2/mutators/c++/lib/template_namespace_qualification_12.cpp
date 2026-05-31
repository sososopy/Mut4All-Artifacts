//source file
#include "../include/template_namespace_qualification_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
        //Filter nodes in header files
        if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                DG->getLocation()))
            return;

        //Get the source code text of target node
        auto deductionGuideText = stringutils::rangetoStr(*(Result.SourceManager),
                                                          DG->getSourceRange());
        //Determine the namespace for the associated template
        if (auto *NSD = dyn_cast<NamespaceDecl>(DG->getDeclContext())) {
            std::string namespacePrefix = NSD->getNameAsString() + "::";
            //Perform mutation on the source code text by applying string replacement
            size_t pos = deductionGuideText.find("->");
            if (pos != std::string::npos) {
                deductionGuideText.insert(0, namespacePrefix);
                deductionGuideText.insert(pos + 2, namespacePrefix);
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(DG->getSourceRange()), deductionGuideText);
        }
    }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDeductionGuideDecl().bind("DeductionGuide");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}