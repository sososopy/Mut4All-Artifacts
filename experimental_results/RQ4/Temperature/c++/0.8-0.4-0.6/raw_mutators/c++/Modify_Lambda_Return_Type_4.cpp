//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_return_type_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_return_type_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;

        const auto *LambdaType = LE->getLambdaClass()->getLambdaTypeInfo();
        if (!LambdaType)
            return;

        const FunctionProtoType *FPT = LambdaType->getType()->getAs<FunctionProtoType>();
        if (!FPT)
            return;

        QualType ReturnType = FPT->getReturnType();
        if (ReturnType->isSpecificBuiltinType(BuiltinType::Int) || ReturnType->isSpecificBuiltinType(BuiltinType::Float)) {
            std::string lambdaSource = Lexer::getSourceText(CharSourceRange::getTokenRange(LE->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());
            std::string modifiedLambda = lambdaSource;

            size_t arrowPos = modifiedLambda.find("->");
            if (arrowPos != std::string::npos) {
                size_t bracePos = modifiedLambda.find("{", arrowPos);
                if (bracePos != std::string::npos) {
                    modifiedLambda.replace(arrowPos, bracePos - arrowPos, "-> int(int)");
                    size_t returnPos = modifiedLambda.find("return", bracePos);
                    if (returnPos != std::string::npos) {
                        modifiedLambda.insert(returnPos, "auto tempFunc = [](int y) { return x * y; }; ");
                        size_t semicolonPos = modifiedLambda.find(";", returnPos);
                        if (semicolonPos != std::string::npos) {
                            modifiedLambda.replace(returnPos, semicolonPos - returnPos, "return tempFunc;");
                        }
                    }
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), modifiedLambda);
        }
    }
}

void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}