//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_With_External_Constexpr_465
 */ 
class MutatorFrontendAction_465 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(465)

private:
    class MutatorASTConsumer_465 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_465(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::map<std::string, std::string> staticMemberMap;
    };
};

//source file
#include "../include/replace_static_member_with_external_constexpr_465.h"

// ========================================================================================================
#define MUT465_OUTPUT 1

void MutatorFrontendAction_465::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VariableTemplate")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        auto *TT = VD->getType()->getAs<clang::TemplateSpecializationType>();
        if (!TT)
            return;

        const clang::TemplateArgument &Arg = TT->getArg(0);
        if (Arg.getKind() != clang::TemplateArgument::Expression)
            return;

        auto *E = Arg.getAsExpr();
        if (auto *ME = llvm::dyn_cast<clang::MemberExpr>(E)) {
            if (auto *DRE = llvm::dyn_cast<clang::DeclRefExpr>(ME->getBase())) {
                auto *CD = llvm::dyn_cast<clang::CXXRecordDecl>(DRE->getDecl()->getType()->getAs<clang::RecordType>()->getDecl());
                if (CD && CD->isCompleteDefinition()) {
                    for (auto *M : CD->decls()) {
                        if (auto *VD = llvm::dyn_cast<clang::VarDecl>(M)) {
                            if (VD->isStaticDataMember() && VD->isConstexpr()) {
                                std::string staticMemberName = VD->getNameAsString();
                                std::string externalConstexpr = "ExternalValue_" + staticMemberName;
                                staticMemberMap[staticMemberName] = externalConstexpr;
                                Rewrite.ReplaceText(ME->getSourceRange(), externalConstexpr);
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_465::MutatorASTConsumer_465::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("VariableTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}