//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_enum_to_forward_declaration_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
        const EnumDecl *enumDecl = nullptr;
    };
};

//source file
#include "../include/modify_enum_to_forward_declaration_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      
      if (ED->isCompleteDefinition()) {
        //Get the source code text of target node
        auto enumName = ED->getNameAsString();
        //Perform mutation on the source code text by applying string replacement
        std::string forwardDecl = "enum " + enumName + ";";
        Rewrite.ReplaceText(ED->getSourceRange(), forwardDecl);

        // Insert usage to trigger incomplete type error
        std::string usage = "\n/*mut36*/template <typename T>\nstruct CheckEnum {\n"
                            "    static_assert(sizeof(T) > 0, \"Incomplete type\");\n};\n"
                            "CheckEnum<" + enumName + "> instance;\n";
        Rewrite.InsertTextAfter(ED->getEndLoc(), usage);
      }
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}