//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Attribute_With_Statement_Expression_On_Lambda_174
 */ 
class MutatorFrontendAction_174 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(174)

private:
    class MutatorASTConsumer_174 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_174(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::VarDecl *> availableVars;
        std::vector<const clang::TypeDecl *> availableTypes;
    };
};

//source file
#include "../include/Mutator_Insert_Attribute_With_Statement_Expression_On_Lambda_174.h"

// ========================================================================================================
#define MUT174_OUTPUT 1

void MutatorFrontendAction_174::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        //Record available variables for statement expression
        availableVars.push_back(VD);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        //Record available types for statement expression
        availableTypes.push_back(TD);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getBeginLoc()))
            return;
        //Get the source code text of target node
        SourceRange lambdaRange = MT->getSourceRange();
        std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
        
        //Select attribute name from predefined list
        std::vector<std::string> attrNames = {"b", "alias", "if", "assume"};
        int attrIdx = getrandom::getRandomIndex(attrNames.size() - 1);
        std::string attrName = attrNames[attrIdx];
        
        //Construct statement expression argument
        std::string stmtExpr = "({ ";
        //Use an existing variable if available
        if (!availableVars.empty()) {
            int varIdx = getrandom::getRandomIndex(availableVars.size() - 1);
            const clang::VarDecl *var = availableVars[varIdx];
            std::string varName = var->getNameAsString();
            stmtExpr += varName + " = 0; ";
        } else if (!availableTypes.empty()) {
            //Use an existing type to declare a variable
            int typeIdx = getrandom::getRandomIndex(availableTypes.size() - 1);
            const clang::TypeDecl *type = availableTypes[typeIdx];
            std::string typeName = type->getNameAsString();
            stmtExpr += typeName + " x = 0; ";
        } else {
            //Fallback to primitive type
            stmtExpr += "int x = 0; ";
        }
        stmtExpr += "})";
        
        //Construct attribute string
        std::string attribute = "__attribute__((" + attrName + "((" + stmtExpr + "))))";
        
        //Find insertion point: after parameter list or capture list
        SourceLocation insertLoc;
        if (MT->hasExplicitParameters()) {
            //Insert after parameter list
            const clang::CXXMethodDecl *callOperator = MT->getCallOperator();
            if (callOperator) {
                insertLoc = callOperator->getBeginLoc();
            } else {
                insertLoc = MT->getBody()->getBeginLoc();
            }
        } else {
            //Insert after capture list
            insertLoc = MT->getBody()->getBeginLoc();
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = lambdaText;
        //Insert attribute before lambda body
        SourceManager &SM = Rewrite.getSourceMgr();
        std::string locText = stringutils::rangetoStr(SM, SourceRange(insertLoc, insertLoc));
        size_t insertPos = mutatedText.find(locText);
        if (insertPos != std::string::npos) {
            mutatedText.insert(insertPos, " " + attribute + " ");
        } else {
            //Fallback: insert at beginning of body
            size_t bodyPos = mutatedText.find("{");
            if (bodyPos != std::string::npos) {
                mutatedText.insert(bodyPos, " " + attribute + " ");
            }
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedText);
    }
}
  
void MutatorFrontendAction_174::MutatorASTConsumer_174::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher typeMatcher = typeDecl(unless(isImplicit())).bind("TypeDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}