//source file
#include "../include/modify_function_template_with_concept_constraints_68.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        if (auto *CT = FT->getTemplatedDecl()) {
            if (auto *params = FT->getTemplateParameters()) {
                std::string newConcept = "RecursiveConcept";
                std::string originalConcept = params->getParam(0)->getNameAsString();
                std::string replacement = "template<typename T> concept " + newConcept + " = " + originalConcept + " && requires(T t) { anotherFunction(t); };";

                std::string functionTemplateText = stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
                size_t pos = functionTemplateText.find(originalConcept);
                if (pos != std::string::npos) {
                    functionTemplateText.replace(pos, originalConcept.length(), newConcept);
                    functionTemplateText = replacement + "\n" + functionTemplateText;
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), functionTemplateText);
                }
            }
        }
    }
}

void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}