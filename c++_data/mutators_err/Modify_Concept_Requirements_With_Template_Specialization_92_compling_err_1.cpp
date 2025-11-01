//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Requirements_With_Template_Specialization_92
 */ 
class MutatorFrontendAction_92 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(92)

private:
    class MutatorASTConsumer_92 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_92(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_concept_requirements_with_template_specialization_92.h"

// ========================================================================================================
#define MUT92_OUTPUT 1

void MutatorFrontendAction_92::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *ConceptDecl = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
        // Filter nodes in header files
        if (!ConceptDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                ConceptDecl->getLocation()))
            return;

        // Get the source code text of target node
        auto conceptSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                     ConceptDecl->getSourceRange());

        // Perform mutation by introducing a new template specialization
        std::string mutatedConcept = conceptSource;
        mutatedConcept += "\n/*mut92*/template<typename T>\n";
        mutatedConcept += "struct example_specialization<T, std::enable_if_t<!example_concept<T>>> {\n";
        mutatedConcept += "    // mutated behavior\n";
        mutatedConcept += "};\n";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ConceptDecl->getSourceRange()), mutatedConcept);
    }
}
  
void MutatorFrontendAction_92::MutatorASTConsumer_92::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}