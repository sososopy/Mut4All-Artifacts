//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Invalid_Member_Access_Through_Comma_Operator_In_Sizeof_Expression_300
 */ 
class MutatorFrontendAction_300 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(300)

private:
    class MutatorASTConsumer_300 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_300(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> nonStructVariables;
        std::vector<std::string> memberNames;
    };
};

//source file
#include "../include/Mutator_Introduce_Invalid_Member_Access_Through_Comma_Operator_In_Sizeof_Expression_300.h"

// ========================================================================================================
#define MUT300_OUTPUT 1

void MutatorFrontendAction_300::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnaryExprOrTypeTraitExpr>("SizeofExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      if (MT->getKind() != clang::UETT_SizeOf) return;
      
      //Get the source code text of target node
      auto expr = MT->getArgumentExpr();
      if (!expr) return;
      
      //Check if it's a comma operator expression
      if (auto *BO = dyn_cast<clang::BinaryOperator>(expr)) {
        if (BO->getOpcode() != clang::BO_Comma) return;
        
        clang::Expr *rightExpr = BO->getRHS();
        
        //Check if right operand is struct/class type
        clang::QualType rightType = rightExpr->getType();
        if (!rightType->isStructureType() && !rightType->isClassType()) return;
        
        //Find member names from the struct/class
        const clang::RecordType *recordType = rightType->getAs<clang::RecordType>();
        if (!recordType) return;
        const clang::RecordDecl *recordDecl = recordType->getDecl();
        if (!recordDecl) return;
        
        //Collect member names
        memberNames.clear();
        for (auto *field : recordDecl->fields()) {
            memberNames.push_back(field->getNameAsString());
        }
        if (memberNames.empty()) return;
        
        //Find non-struct variable or create literal
        std::string nonStructExpr;
        if (nonStructVariables.empty()) {
            nonStructExpr = "0"; //int literal
        } else {
            size_t idx = getrandom::getRandomIndex(nonStructVariables.size() - 1);
            nonStructExpr = nonStructVariables[idx]->getNameAsString();
        }
        
        //Choose random member name
        size_t memberIdx = getrandom::getRandomIndex(memberNames.size() - 1);
        std::string memberName = memberNames[memberIdx];
        
        //Perform mutation on the source code text by applying string replacement
        std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
        std::string mutatedText = "sizeof(" + nonStructExpr + "." + memberName + ")";
        Rewrite.ReplaceText(MT->getSourceRange(), mutatedText);
      }
    }
}

void MutatorFrontendAction_300::MutatorASTConsumer_300::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder Finder;
    Callback callback(TheRewriter);
    Finder.addMatcher(
        unaryExprOrTypeTraitExpr(
            hasArgument(expr().bind("SizeofExpr"))
        ).bind("SizeofExpr"),
        &callback
    );
    Finder.matchAST(Context);
}