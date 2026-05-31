//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enum_With_Class_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)

private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_enum_with_class_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        //Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;

        // Get the source code text of target node
        auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string classSource = "class " + ED->getNameAsString() + " {\npublic:\n";
        for (auto *E : ED->enumerators()) {
            classSource += "static const int " + E->getNameAsString() + " = " + std::to_string(E->getInitVal().getSExtValue()) + ";\n";
        }
        classSource += "};\n";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), classSource);
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}