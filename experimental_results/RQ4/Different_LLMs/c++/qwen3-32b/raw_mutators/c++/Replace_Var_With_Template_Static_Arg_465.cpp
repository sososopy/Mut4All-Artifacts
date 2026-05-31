//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Var_With_Template_Static_Arg_465
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/Replace_Var_With_Template_Static_Arg_465.h"

// ========================================================================================================
#define MUT465_OUTPUT 1

void MutatorFrontendAction_465::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        if (VD->isStatic())
            return;
        auto *RD = VD->getDeclContext()->getRecordDecl();
        if (!RD || !RD->isTemplateInstantiation())
            return;
        auto *Init = VD->getInit();
        if (!Init)
            return;
        auto *BO = Init->IgnoreImplicitConversionsAndCasts()->getAs<clang::BinaryOperator>();
        if (!BO)
            return;
        const clang::VarDecl *staticMember = nullptr;
        const clang::VarDecl *otherOperand = nullptr;
        bool found = false;
        {
            auto *LHS = BO->getLHS()->IgnoreImplicitConversionsAndCasts();
            auto *RHS = BO->getRHS()->IgnoreImplicitConversionsAndCasts();
            if (auto *LDeclRef = dyn_cast<clang::DeclRefExpr>(LHS)) {
                if (auto *LV = dyn_cast<clang::VarDecl>(LDeclRef->getDecl())) {
                    if (LV->isStatic() && LV->getDeclContext()->getRecordDecl() == RD) {
                        staticMember = LV;
                        if (auto *RDeclRef = dyn_cast<clang::DeclRefExpr>(RHS)) {
                            otherOperand = dyn_cast<clang::VarDecl>(RDeclRef->getDecl());
                            found = true;
                        }
                    }
                }
            }
        }
        if (!found) {
            auto *RDeclRef = dyn_cast<clang::DeclRefExpr>(BO->getRHS()->IgnoreImplicitConversionsAndCasts());
            if (RDeclRef) {
                if (auto *RV = dyn_cast<clang::VarDecl>(RDeclRef->getDecl())) {
                    if (RV->isStatic() && RV->getDeclContext()->getRecordDecl() == RD) {
                        staticMember = RV;
                        if (auto *LDeclRef = dyn_cast<clang::DeclRefExpr>(BO->getLHS()->IgnoreImplicitConversionsAndCasts())) {
                            otherOperand = dyn_cast<clang::VarDecl>(LDeclRef->getDecl());
                            found = true;
                        }
                    }
                }
            }
        }
        if (!found)
            return;
        std::string staticName = staticMember->getNameAsString();
        std::string otherName = otherOperand->getNameAsString();
        std::string varName = VD->getNameAsString();
        clang::QualType staticType = staticMember->getType();
        clang::QualType otherType = otherOperand->getType();
        std::string staticTypeStr = staticType.getUnqualifiedType().getAsString();
        std::string otherTypeStr = otherType.getUnqualifiedType().getAsString();
        std::string templateParamA = "const " + staticTypeStr + "& A";
        std::string templateParamB = "const " + otherTypeStr + "& B";
        std::string opSymbol = BO->getOpcodeStr();
        std::string templateDecl = "template<" + templateParamA + ", " + templateParamB + ">\nstatic constexpr auto Concat = A " + opSymbol + " B;\n";
        std::string newInit = "Concat<" + staticName + ", " + otherName + ">";
        auto sourceRange = VD->getSourceRange();
        auto originalCode = stringutils::rangetoStr(*Result.SourceManager, sourceRange);
        std::string newCode = originalCode;
        size_t equalsPos = newCode.find("=");
        if (equalsPos != std::string::npos) {
            newCode.replace(equalsPos + 1, newCode.length() - equalsPos - 1, newInit);
        }
        std::string fullReplacement = templateDecl + newCode;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), fullReplacement);
        cur_classes.push_back(RD);
    }
}

void MutatorFrontendAction_465::MutatorASTConsumer_465::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(
        inClassStyle,
        hasDeclContext(cxxRecordDecl(isTemplateInstantiation())),
        hasInitializer(ignoringParenImpCasts(binaryOperator()))
    ).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}