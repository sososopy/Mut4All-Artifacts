//source file
#include "../include/modify_inheritance_and_access_specifiers_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedStruct")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;

        derivedStructs.push_back(Derived);

        for (auto &BaseSpecifier : Derived->bases()) {
            if (BaseSpecifier.isVirtual()) {
                SourceRange BaseRange = BaseSpecifier.getSourceRange();
                std::string BaseText = stringutils::rangetoStr(*(Result.SourceManager), BaseRange);
                std::string MutatedBaseText = BaseText;
                size_t pos = MutatedBaseText.find("virtual ");
                if (pos != std::string::npos) {
                    MutatedBaseText.erase(pos, 8);
                }
                Rewrite.ReplaceText(BaseRange, MutatedBaseText);
            }
        }
    } else if (auto *MemberExpr = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberAccess")) {
        if (!MemberExpr || !Result.Context->getSourceManager().isWrittenInMainFile(MemberExpr->getExprLoc()))
            return;

        if (auto *Base = MemberExpr->getBase()) {
            if (auto *DRE = dyn_cast<clang::DeclRefExpr>(Base->IgnoreImpCasts())) {
                if (auto *VD = dyn_cast<clang::VarDecl>(DRE->getDecl())) {
                    for (auto *Derived : derivedStructs) {
                        if (Derived->getNameAsString() == VD->getType().getAsString()) {
                            SourceRange MemberRange = MemberExpr->getSourceRange();
                            std::string MemberText = stringutils::rangetoStr(*(Result.SourceManager), MemberRange);
                            size_t pos = MemberText.find("::");
                            if (pos != std::string::npos) {
                                MemberText.erase(pos, 2);
                            }
                            Rewrite.ReplaceText(MemberRange, MemberText);
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(cxxRecordDecl(hasAnyBase(cxxBaseSpecifier(isVirtual()))))).bind("DerivedStruct");
    StatementMatcher memberAccessMatcher = memberExpr(hasAncestor(cxxRecordDecl())).bind("MemberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.addMatcher(memberAccessMatcher, &callback);
    matchFinder.matchAST(Context);
}