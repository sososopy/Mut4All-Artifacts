//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * modify_transparent_union_declaration_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/mutator_modify_transparent_union_declaration_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("typedefWithTransparentUnion")) {
        //Filter nodes in header files
        if (!TD || !Result.SourceManager->isWrittenInMainFile(TD->getLocation()))
            return;

        //Get the source code text of target node
        auto typedefRange = TD->getSourceRange();
        std::string typedefText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(typedefRange),
                                                              *Result.SourceManager, 
                                                              Result.Context->getLangOpts()).str();

        //Perform mutation on the source code text by applying string replacement
        size_t pos = typedefText.find("__attribute__((__transparent_union__))");
        if (pos != std::string::npos) {
            typedefText.erase(pos, std::string("__attribute__((__transparent_union__))").length());
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(typedefRange, typedefText);
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::typedefDecl(
        clang::ast_matchers::hasType(clang::ast_matchers::qualType(clang::ast_matchers::hasDeclaration(
            clang::ast_matchers::recordDecl(clang::ast_matchers::isUnion())))),
        clang::ast_matchers::hasAttr(clang::attr::TransparentUnion)).bind("typedefWithTransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}