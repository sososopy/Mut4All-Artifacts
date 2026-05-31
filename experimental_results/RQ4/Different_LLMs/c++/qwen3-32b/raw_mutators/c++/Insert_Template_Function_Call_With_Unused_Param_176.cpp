//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Function_Call_With_Unused_Param_176
 */ 
class MutatorFrontendAction_176 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(176)
private:
    class MutatorASTConsumer_176 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_176(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<FunctionDecl*> candidates;
    };
};

//source file
#include "../include/Insert_Template_Function_Call_With_Unused_Param_176.h"

// ========================================================================================================
#define MUT176_OUTPUT 1

void MutatorFrontendAction_176::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;
        if (FD->getBody()->isEmpty()) {
            ParmVarDecl *param = FD->getParamDecl(0);
            if (param && param->getType()->isRValueReferenceType()) {
                QualType pointeeType = param->getType().getPointeeType();
                if (pointeeType->getAs<TemplateTypeParmType>()) {
                    candidates.push_back(FD);
                }
            }
        }
    } else if (auto *callerFD = Result.Nodes.getNodeAs<FunctionDecl>("callerFunc")) {
        if (!callerFD || !Result.Context->getSourceManager().isWrittenInMainFile(callerFD->getLocation()))
            return;
        if (!candidates.empty() && callerFD->hasBody()) {
            FunctionDecl *selectedFD = candidates[0];
            std::string funcName = selectedFD->getNameAsString();
            std::string callStmt = funcName + "(0);  // Added call with temporary argument\n";
            CompoundStmt *body = callerFD->getBody();
            SourceLocation endLoc = body->getRBracLoc();
            Rewrite.InsertText(endLoc, "\n    " + callStmt, true, true);
        }
    }
}
  
void MutatorFrontendAction_176::MutatorASTConsumer_176::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcDeclMatcher = 
        functionDecl(
            hasParent(functionTemplateDecl()),
            hasParameter(0, hasType(qualType(references(2, hasCanonicalType(templateTypeParmType()))))),
            hasBody(compoundStmt(isEmpty()))
        ).bind("funcDecl");
    DeclarationMatcher callerFuncMatcher = functionDecl().bind("callerFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcDeclMatcher, &callback);
    matchFinder.addMatcher(callerFuncMatcher, &callback);
    matchFinder.matchAST(Context);
}