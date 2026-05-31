//source file
#include "../include/Modify_Enum_Parameter_Syntax_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (auto param : FD->parameters()) {
            if (auto *ET = param->getType()->getAs<clang::EnumType>()) {
                if (ET->getDecl()->getDeclContext() == FD->getDeclContext()) {
                    auto paramSourceRange = param->getSourceRange();
                    std::string paramText = stringutils::rangetoStr(*(Result.SourceManager), paramSourceRange);
                    paramText.insert(paramText.find(param->getNameAsString()), ";");
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramSourceRange), paramText);
                }
            }
        }
    }
}

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(hasParameter(0, hasType(enumType()))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}