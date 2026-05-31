//header file
#pragma once
#include "Mutator_base.h"

/**
 * conditional_expression_with_struct_member_access_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/conditional_expression_with_struct_member_access_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *MD = Result.Nodes.getNodeAs<clang::PreprocessorDirective>("Macro")) {
            auto macroText = stringutils::rangetoStr(*(Result.SourceManager), MD->getSourceRange());

            if (FD->getNumParams() > 0) {
                if (auto *PVD = dyn_cast<ParmVarDecl>(FD->getParamDecl(0))) {
                    if (auto *RT = dyn_cast<clang::PointerType>(PVD->getType().getTypePtr())) {
                        if (auto *RD = dyn_cast<clang::RecordType>(RT->getPointeeType().getTypePtr())) {
                            if (RD->getDecl()->isStruct()) {
                                auto structName = RD->getDecl()->getNameAsString();
                                auto memberName = "member"; // Assuming member is known or can be retrieved
                                std::string mutation = "constexpr int result = " + macroText + "(10, " + PVD->getNameAsString() + "->" + memberName + ");";
                                Rewrite.InsertTextAfterToken(FD->getBody()->getBeginLoc(), "\n/*mut21*/" + mutation + "\n");
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto macroMatcher = preprocessorDirective().bind("Macro");
    auto functionMatcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}