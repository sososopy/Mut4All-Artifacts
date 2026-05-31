//header file
#pragma once
#include "Mutator_base.h"

/**
 * enum_declaration_in_function_parameter_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/enum_declaration_in_function_parameter_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithEnumParam")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        // Get the parameter list source range
        auto paramRange = FD->getSourceRange();
        auto paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);

        // Perform mutation by replacing comma with semicolon in parameter list
        size_t pos = paramText.find(",");
        if (pos != std::string::npos) {
            paramText.replace(pos, 1, ";");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), paramText);
        }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify functions with enum parameters within class or struct
    DeclarationMatcher matcher = functionDecl(
        hasParameter(0, hasType(enumType())),
        hasParent(anyOf(cxxRecordDecl(), recordDecl()))
    ).bind("FunctionWithEnumParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}