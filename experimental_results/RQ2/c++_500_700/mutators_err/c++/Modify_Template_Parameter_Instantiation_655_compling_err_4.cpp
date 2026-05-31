//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_parameter_instantiation_655
 */ 
class MutatorFrontendAction_655 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(655)

private:
    class MutatorASTConsumer_655 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_655(Rewriter &R) : TheRewriter(R) {}
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
#include "MutatorFrontendAction_655.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;
using namespace ast_matchers;

void MutatorFrontendAction_655::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        // Check if the specialization has default template arguments
        if (CT->getSpecializationKind() != TSK_ExplicitSpecialization) {
            // Get the source code text of target node
            auto specialization = TheRewriter.getRewrittenText(CT->getSourceRange());

            // Perform mutation by introducing a new template specialization
            std::string newSpecialization = specialization;
            newSpecialization.replace(newSpecialization.find("SomeImpl"), 8, "OtherImpl");

            // Replace the original AST node with the mutated one
            TheRewriter.ReplaceText(CT->getSourceRange(), newSpecialization);
        }
    }
}

void MutatorFrontendAction_655::MutatorASTConsumer_655::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}