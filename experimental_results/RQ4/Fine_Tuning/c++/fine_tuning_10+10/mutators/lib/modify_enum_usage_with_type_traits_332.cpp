//source file
#include "../include/Modify_Enum_Usage_With_Type_Traits_332.h"
#include <vector>

// ========================================================================================================
#define MUT332_OUTPUT 1

void MutatorFrontendAction_332::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;

        for (auto *EnumConstant : ED->enumerators()) {
            if (EnumConstant) {
                std::string enumName = ED->getNameAsString();
                std::string constName = EnumConstant->getNameAsString();
                std::string replacement = "std::integral_constant<" + enumName + ", " + constName + ">";

                const auto *use = Result.Nodes.getNodeAs<clang::DeclRefExpr>("EnumUse");
                if (use && use->getDecl() == EnumConstant) {
                    Rewrite.ReplaceText(use->getSourceRange(), replacement);
                }
            }
        }
    }
}

void MutatorFrontendAction_332::MutatorASTConsumer_332::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    StatementMatcher useMatcher = declRefExpr(to(enumConstantDecl())).bind("EnumUse");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(useMatcher, &callback);
    matchFinder.matchAST(Context);
}