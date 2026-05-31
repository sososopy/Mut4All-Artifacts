//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_NonType_Param_439
 */ 
class MutatorFrontendAction_439 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(439)

private:
    class MutatorASTConsumer_439 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_439(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Specialization_NonType_Param_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;

        const TemplateArgument &arg = TS->getTemplateArgs().get(0);
        if (arg.getKind() == TemplateArgument::Integral) {
            llvm::APInt intValue = arg.getAsIntegral();
            std::string originalValue = intValue.toString(10, true);
            std::string mutatedValue = "(" + originalValue + " + 0)";
            
            SourceLocation startLoc = TS->getLocation();
            SourceLocation endLoc = TS->getRBraceLoc();
            if (startLoc.isValid() && endLoc.isValid()) {
                std::string replacement = "template<> struct " + TS->getNameAsString() + "<" + mutatedValue + "> {";
                Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
            }
        }
    }
}

void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(has(templateArgumentCountIs(1))).bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}