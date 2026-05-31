//source file
#include "../include/modify_crtp_with_consteval_and_static_cast_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("CRTPMethod")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isConstexpr()) {
            auto body = MT->getBody();
            if (body) {
                std::string bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
                size_t pos = bodyStr.find("static_cast<");
                if (pos != std::string::npos) {
                    if (bodyStr.find("(*this)") != std::string::npos) {
                        bodyStr.replace(bodyStr.find("(*this)"), 7, "(this)");
                    } else if (bodyStr.find("(this)") != std::string::npos) {
                        bodyStr.replace(bodyStr.find("(this)"), 6, "(*this)");
                    }
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), bodyStr);
                }
            }
        } else {
            auto methodDecl = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
            size_t pos = methodDecl.find("{");
            if (pos != std::string::npos) {
                methodDecl.insert(pos, " consteval");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), methodDecl);
            }
        }
    }
}

void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(hasDescendant(classTemplateSpecializationDecl(hasAncestor(classTemplateDecl())))), isDefinition()).bind("CRTPMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}