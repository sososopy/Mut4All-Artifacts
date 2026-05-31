//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Call_With_Nested_Template_175
 */ 
class MutatorFrontendAction_175 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(175)

private:
    class MutatorASTConsumer_175 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_175(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Replace_Function_Call_With_Nested_Template_175.h"

// ========================================================================================================
#define MUT175_OUTPUT 1

void MutatorFrontendAction_175::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!call || !Result.Context->getSourceManager().isWrittenInMainFile(call->getBeginLoc()))
            return;

        FunctionDecl *funcDecl = call->getEnclosingFunctionDecl();
        if (auto methodDecl = dyn_cast<clang::CXXMethodDecl>(funcDecl)) {
            clang::CXXRecordDecl *recordDecl = methodDecl->getParent();
            if (recordDecl->getTemplateParameters()) {
                std::string outerName = recordDecl->getNameAsString();
                clang::TemplateParameterList *tparams = recordDecl->getTemplateParameters();
                std::string tparams_str;
                if (tparams) {
                    for (unsigned i = 0; i < tparams->size(); ++i) {
                        clang::TemplateParameter *tparam = tparams->getParam(i);
                        if (i > 0) tparams_str += ", ";
                        if (auto *tt = clang::dyn_cast<clang::TemplateTypeParmDecl>(tparam)) {
                            tparams_str += tt->getNameAsString();
                        } else if (auto *nt = clang::dyn_cast<clang::NonTypeTemplateParmDecl>(tparam)) {
                            tparams_str += nt->getNameAsString();
                        }
                    }
                }
                std::string outerWithParams = outerName + "<" + tparams_str + ">";
                std::string replacement = outerWithParams + "::Inner<1>()";
                clang::SourceLocation start = call->getBeginLoc();
                clang::SourceLocation end = call->getEndLoc();
                Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(start, end), replacement);
            }
        }
    }
}
  
void MutatorFrontendAction_175::MutatorASTConsumer_175::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::DeclarationMatcher matcher = clang::callExpr(
        clang::hasAncestor(clang::cxxMethodDecl(
            clang::hasAncestor(clang::cxxRecordDecl(
                clang::hasTemplateParameterList()
            ))
        ))
    ).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}