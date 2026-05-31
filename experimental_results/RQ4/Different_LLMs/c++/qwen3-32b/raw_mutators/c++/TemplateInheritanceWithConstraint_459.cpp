//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInheritanceWithConstraint_459
 */ 
class MutatorFrontendAction_TemplateInheritanceWithConstraint_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_TemplateInheritanceWithConstraint_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInheritanceWithConstraint_459(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInheritanceWithConstraint_459.h"

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_TemplateInheritanceWithConstraint_459::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *structDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!structDecl || !Result.Context->getSourceManager().isWrittenInMainFile(structDecl->getLocation()))
            return;

        std::string structName = structDecl->getNameAsString();
        if (structName.empty())
            return;

        // Get the source range of the struct
        SourceRange structRange = structDecl->getSourceRange();
        if (structRange.isInvalid())
            return;

        // Generate the new code for the template struct
        std::string newStructCode = "template<typename T>\nstruct " + structName + " {\n  " + structName + "() requires (IsSame<T, int>);\n};";

        // Replace the original struct's code with the new code
        Rewrite.ReplaceText(structRange, newStructCode);

        // Generate the specialization code
        std::string specializationCode = "\ntemplate<> struct " + structName + "<void> : public " + structName + "<int> { using " + structName + "<int>::" + structName + "; };";

        // Insert the specialization code after the original struct's end location
        SourceLocation endLoc = structDecl->getEndLoc();
        Rewrite.InsertTextAfter(endLoc, specializationCode);
    }
}

void MutatorFrontendAction_TemplateInheritanceWithConstraint_459::MutatorASTConsumer_TemplateInheritanceWithConstraint_459::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxConstructorDecl(isDefaultConstructor()))).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}