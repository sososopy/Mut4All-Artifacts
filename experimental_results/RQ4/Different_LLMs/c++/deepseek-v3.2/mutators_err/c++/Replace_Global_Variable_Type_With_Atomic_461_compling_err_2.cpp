//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Global_Variable_Type_With_Atomic_461
 */ 
class MutatorFrontendAction_461 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(461)

private:
    class MutatorASTConsumer_461 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_461(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Replace_Global_Variable_Type_With_Atomic_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("GlobalVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if variable is global
      if (!VD->isFileVarDecl())
        //Check if type is scalar or pointer
        QualType QT = VD->getType();
        if (!QT->isScalarType() && !QT->isPointerType())
          return;
        //Check if type is trivially copyable (for C++ compatibility)
        if (QT->isRecordType()) {
          const RecordType *RT = QT->getAs<RecordType>();
          if (RT) {
            const CXXRecordDecl *RD = dyn_cast<CXXRecordDecl>(RT->getDecl());
            if (RD && !RD->isTriviallyCopyable())
              return;
          }
        }
        //Get the source code text of target node
        std::string OriginalTypeStr = VD->getType().getAsString();
        std::string NewTypeStr = "_Atomic " + OriginalTypeStr;
        //Handle initializer
        const Expr *InitExpr = VD->getInit();
        std::string NewInitStr = "";
        if (InitExpr) {
          //Check if initializer is constant
          if (InitExpr->isConstantInitializer(*Result.Context, false)) {
            //Keep original initializer
            NewInitStr = " = " + stringutils::rangetoStr(*(Result.SourceManager), InitExpr->getSourceRange());
          } else {
            //Replace with compatible constant
            if (QT->isPointerType())
              NewInitStr = " = nullptr";
            else if (QT->isIntegerType())
              NewInitStr = " = 0";
            else if (QT->isFloatingType())
              NewInitStr = " = 0.0";
            else
              NewInitStr = " = 0";
          }
        }
        //Perform mutation on the source code text by applying string replacement
        std::string VarName = VD->getNameAsString();
        std::string NewDeclStr = NewTypeStr + " " + VarName + NewInitStr;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), NewDeclStr);
    }
}
  
void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isDefinition(), hasGlobalStorage()).bind("GlobalVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}