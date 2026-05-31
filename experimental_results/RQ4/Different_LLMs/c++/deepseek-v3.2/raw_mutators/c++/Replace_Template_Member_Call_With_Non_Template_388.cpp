//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Member_Call_With_Non_Template_388
 */ 
class MutatorFrontendAction_388 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(388)

private:
    class MutatorASTConsumer_388 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_388(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Template_Member_Call_With_Non_Template_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto memberName = MT->getMemberNameInfo().getName().getAsString();
      auto baseExpr = MT->getBase();
      if (!baseExpr) return;
      
      auto baseType = baseExpr->getType();
      if (baseType.isNull()) return;
      
      auto recordType = baseType->getAsCXXRecordDecl();
      if (!recordType) return;
      
      //Check if the member is a template function call without parentheses
      auto memberDecl = MT->getMemberDecl();
      if (!memberDecl) return;
      
      auto funcDecl = dyn_cast<FunctionDecl>(memberDecl);
      if (!funcDecl) return;
      
      //Check if it's a template function
      auto templateDecl = funcDecl->getTemplateSpecializationInfo();
      if (!templateDecl) return;
      
      //Find a non-template member function in the same class
      std::vector<FunctionDecl*> nonTemplateFuncs;
      for (auto decl : recordType->decls()) {
        if (auto func = dyn_cast<FunctionDecl>(decl)) {
          if (!func->getTemplateSpecializationInfo() && func->isInstanceMember()) {
            nonTemplateFuncs.push_back(func);
          }
        }
      }
      
      if (nonTemplateFuncs.empty()) return;
      
      //Select a random non-template function
      size_t idx = getrandom::getRandomIndex(nonTemplateFuncs.size() - 1);
      auto targetFunc = nonTemplateFuncs[idx];
      auto targetName = targetFunc->getNameAsString();
      
      //Perform mutation on the source code text by applying string replacement
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      std::string mutatedText = originalText;
      
      //Replace template function name with non-template function name and remove template arguments
      size_t templatePos = mutatedText.find(memberName);
      if (templatePos != std::string::npos) {
        mutatedText.replace(templatePos, memberName.length(), targetName);
        //Remove template arguments if present
        size_t anglePos = mutatedText.find('<');
        if (anglePos != std::string::npos) {
          size_t endAngle = mutatedText.find('>', anglePos);
          if (endAngle != std::string::npos) {
            mutatedText.erase(anglePos, endAngle - anglePos + 1);
          }
        }
      }
      
      mutatedText = "/*mut388*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = memberExpr(hasMemberName("")).bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}