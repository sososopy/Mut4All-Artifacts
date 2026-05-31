//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Enum_Class_Usage_With_Template_Struct_36
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
        const EnumDecl *EnumNode = nullptr;
    };
};

//source file
#include "../include/Invalid_Enum_Class_Usage_With_Template_Struct_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        //Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;
        EnumNode = ED;
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        if (EnumNode) {
            //Get the source code text of target node
            std::string enumName = EnumNode->getNameAsString();
            std::string templateName = TD->getNameAsString();
            std::string mutation = "template <> struct " + templateName + "<" + enumName + "> { static constexpr bool value = false; };";
            mutation += "\nstatic_assert(" + templateName + "<" + enumName + ">::value, \"Unexpected type traits result\");";
            
            //Perform mutation on the source code text by applying string replacement
            Rewrite.InsertTextAfterToken(TD->getEndLoc(), "\n/*mut36*/" + mutation);
        }
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher enumMatcher = enumDecl(isScoped()).bind("EnumDecl");
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}