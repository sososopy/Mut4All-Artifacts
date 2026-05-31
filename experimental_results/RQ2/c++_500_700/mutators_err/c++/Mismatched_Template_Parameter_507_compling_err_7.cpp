//header file
#pragma once
#include "Mutator_base.h"

/**
 * mismatched_template_parameter_507
 */ 
class MutatorFrontendAction_507 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(507)

private:
    class MutatorASTConsumer_507 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_507(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/mismatched_template_parameter_507.h"

// ========================================================================================================
#define MUT507_OUTPUT 1

void MutatorFrontendAction_507::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateClass")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        // Get the template arguments
        const TemplateArgumentList &TemplateArgs = CT->getTemplateArgs();
        if (TemplateArgs.size() > 0) {
            // Perform mutation by introducing a mismatched type
            std::string replacementText = "void*";
            const clang::TemplateArgument &Arg = TemplateArgs[0];
            SourceLocation startLoc, endLoc;

            if (Arg.getKind() == TemplateArgument::Type) {
                QualType type = Arg.getAsType();
                startLoc = type->getLocStart();
                endLoc = type->getLocEnd();
            } else if (Arg.getKind() == TemplateArgument::Integral) {
                startLoc = CT->getLocation();
                endLoc = CT->getLocation();
            }

            // Replace the original template argument with the mismatched type
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacementText);
        }
    }
}

void MutatorFrontendAction_507::MutatorASTConsumer_507::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}