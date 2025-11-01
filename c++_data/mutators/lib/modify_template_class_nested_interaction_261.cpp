//source file
#include "../include/Modify_Template_Class_Nested_Interaction_261.h"

// ========================================================================================================
#define MUT261_OUTPUT 1

void MutatorFrontendAction_261::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isCompleteDefinition() || !TD->isTemplated())
            return;

        for (auto *D : TD->decls()) {
            if (auto *ND = llvm::dyn_cast<clang::CXXRecordDecl>(D)) {
                if (ND->isCompleteDefinition()) {
                    auto nestedContent = stringutils::rangetoStr(*(Result.SourceManager), ND->getSourceRange());
                    std::string mutation = "using BaseType = typename T::NotYetInstantiated;\n";
                    mutation += "void func() { BaseType(); }\n";
                    if (nestedContent.find("func") == std::string::npos) {
                        nestedContent.insert(nestedContent.find("{") + 1, mutation);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ND->getSourceRange()), nestedContent);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_261::MutatorASTConsumer_261::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(has(cxxRecordDecl().bind("TemplateClass"))))).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}