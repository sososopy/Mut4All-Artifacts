//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Constructor_With_List_Init_447
 */ 
class MutatorFrontendAction_447 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(447)

private:
    class MutatorASTConsumer_447 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_447(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_default_constructor_with_list_init_447.h"

// ========================================================================================================
#define MUT447_OUTPUT 1

void MutatorFrontendAction_447::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("constructExpr")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getLocation()))
            return;

        // Only process default constructor calls (no arguments)
        if (CE->getNumArgs() != 0 || CE->isListInitialization())
            return;

        // Get source range of the constructor call
        SourceRange range = CE->getSourceRange();
        if (!range.isValid())
            return;

        // Get original text and perform replacement
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, range);
        std::string newText = originalText;
        
        // Replace () with {}
        size_t openPos = newText.find_last_of('(');
        size_t closePos = newText.find_last_of(')');
        
        if (openPos != std::string::npos && closePos != std::string::npos) {
            newText.replace(openPos, 1, "{");
            newText.replace(closePos, 1, "}");
            Rewrite.ReplaceText(range, newText);
        }
    }
}
  
void MutatorFrontendAction_447::MutatorASTConsumer_447::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match default constructor calls on class types
    DeclarationMatcher matcher = cxxConstructExpr(
        hasDeclaration(cxxConstructorDecl(isDefaultConstructor())),
        hasType(recordType())
    ).bind("constructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}