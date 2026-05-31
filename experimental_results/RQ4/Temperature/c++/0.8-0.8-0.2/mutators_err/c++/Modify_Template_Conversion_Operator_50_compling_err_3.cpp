//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * modify_template_conversion_operator_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
    class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> conversionOperators;
    };
};

//source file
#include "../include/modify_template_conversion_operator_50.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ConversionOperator")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isTemplateInstantiation() && MT->getPrimaryTemplate()) {
            conversionOperators.push_back(MT);
        }
    } else if (auto *MF = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MemberFunction")) {
        if (!MF || !Result.Context->getSourceManager().isWrittenInMainFile(MF->getLocation()))
            return;

        if (MF->hasBody()) {
            std::string body = stringutils::rangetoStr(*(Result.SourceManager), MF->getBody()->getSourceRange());
            
            for (auto *op : conversionOperators) {
                std::string conversionCall = "(void*)(" + op->getNameInfo().getName().getAsString() + "<double>());";
                size_t pos = body.find(op->getNameInfo().getName().getAsString() + "<>");
                if (pos != std::string::npos) {
                    body.replace(pos, op->getNameInfo().getName().getAsString().length() + 2, conversionCall);
                }
            }

            Rewrite.ReplaceText(MF->getBody()->getSourceRange(), "{/*mut50*/\n" + body + "\n}");
        }
    }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    auto conversionOperatorMatcher = cxxMethodDecl(isConversionOperator(), isTemplateInstantiation()).bind("ConversionOperator");
    auto memberFunctionMatcher = cxxMethodDecl(hasBody(stmt())).bind("MemberFunction");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conversionOperatorMatcher, &callback);
    matchFinder.addMatcher(memberFunctionMatcher, &callback);
    matchFinder.matchAST(Context);
}