//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_CRTP_Method_To_ConstEval_59
 */ 
class MutatorFrontendAction_59 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(59)
private:
    class MutatorASTConsumer_59 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_59(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Convert_CRTP_Method_To_ConstEval_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

class CallFinder;

class CallFinder : public clang::RecursiveASTVisitor<CallFinder> {
public:
    CallFinder(clang::Rewriter &R, const std::string &derivedTypeName) : TheRewriter(R), derivedTypeName(derivedTypeName) {}

    bool VisitCXXMemberCallExpr(clang::CXXMemberCallExpr *call) {
        clang::Expr *base = call->getImplicitObjectArgument();
        if (base && base->IgnoreImplicit()->getType()->isPointerType() && 
            base->IgnoreImplicit()->getType()->getPointeeType()->getAs<clang::PointerType>()->getType()->isThisPointerType()) {
            std::string methodName = call->getMemberFunctionDecl()->getNameAsString();
            std::string replacement = "static_cast<" + derivedTypeName + ">(*this)." + methodName + "()";
            TheRewriter.ReplaceText(clang::SourceRange(call->getBeginLoc(), call->getEndLoc()), replacement);
        }
        return true;
    }

private:
    clang::Rewriter &TheRewriter;
    std::string derivedTypeName;
};

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method->hasBody() || method->isConsteval()) return;

        auto *recordDecl = method->getParent();
        if (!recordDecl) return;

        if (auto *classTemplate = recordDecl->getDescribedClassTemplate()) {
            auto templateParams = classTemplate->getTemplateParameters();
            if (templateParams->size() < 1) return;

            auto param = templateParams->getParam(0);
            if (auto *ttParam = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                std::string derivedTypeName = ttParam->getNameAsString();

                SourceLocation startLoc = method->getBeginLoc();
                Rewrite.InsertText(startLoc, "consteval ", true, true);

                auto body = method->getBody();
                if (!body) return;

                CallFinder finder(Rewrite, derivedTypeName);
                finder.TraverseStmt(body);
            }
        }
    }
}
  
void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        hasParent(cxxRecordDecl(clang::ast_matchers::isClassTemplate())),
        unless(isConsteval())
    ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}