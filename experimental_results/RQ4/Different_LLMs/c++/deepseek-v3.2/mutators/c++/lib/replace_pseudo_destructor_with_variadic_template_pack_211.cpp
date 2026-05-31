//source file
#include "../include/Mutator_211.h"

// ========================================================================================================
#define MUT211_OUTPUT 1

void MutatorFrontendAction_211::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PD = Result.Nodes.getNodeAs<CXXMemberCallExpr>("PseudoDestructor")) {
        //Filter nodes in header files
        if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       PD->getBeginLoc()))
            return;
        
        //Get the source code text of target node
        auto exprText = stringutils::rangetoStr(*(Result.SourceManager),
                                                PD->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        //Find the class or function context
        const DeclContext* context = PD->getCalleeDecl()->getDeclContext();
        bool isTemplate = false;
        std::string packName = "Pack";
        
        //Check if context is a template class or function
        if (const CXXRecordDecl* record = dyn_cast<CXXRecordDecl>(context)) {
            if (record->getDescribedClassTemplate()) {
                isTemplate = true;
                //Check if it already has a parameter pack
                auto* templateDecl = record->getDescribedClassTemplate();
                if (templateDecl->getTemplateParameters()->size() > 0) {
                    //Use existing template parameter name if available
                    packName = templateDecl->getTemplateParameters()->getParam(0)->getNameAsString();
                }
            }
        } else if (const FunctionDecl* func = dyn_cast<FunctionDecl>(context)) {
            if (func->getDescribedFunctionTemplate()) {
                isTemplate = true;
                //Check if it already has a parameter pack
                auto* templateDecl = func->getDescribedFunctionTemplate();
                if (templateDecl->getTemplateParameters()->size() > 0) {
                    packName = templateDecl->getTemplateParameters()->getParam(0)->getNameAsString();
                }
            }
        }
        
        //Replace pseudo-destructor call with variadic template pack expansion
        size_t dotPos = exprText.find('.');
        size_t tildePos = exprText.find('~', dotPos);
        if (dotPos != std::string::npos && tildePos != std::string::npos) {
            //Replace Type with Pack...
            std::string mutatedExpr = exprText.substr(0, tildePos + 1) + packName + "...()";
            
            //If not in template context, add template parameter pack to enclosing context
            if (!isTemplate) {
                //Find the nearest class or function to make it a template
                if (const CXXRecordDecl* record = dyn_cast<CXXRecordDecl>(context)) {
                    //Make the class a template with parameter pack
                    std::string classText = stringutils::rangetoStr(*(Result.SourceManager),
                                                                     record->getSourceRange());
                    size_t classPos = classText.find("class ") != std::string::npos ? 
                                      classText.find("class ") : classText.find("struct ");
                    if (classPos != std::string::npos) {
                        std::string templateDecl = "template<typename... " + packName + ">\n";
                        classText.insert(classPos, templateDecl);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                            record->getSourceRange()), classText);
                    }
                } else if (const FunctionDecl* func = dyn_cast<FunctionDecl>(context)) {
                    //Make the function a template with parameter pack
                    std::string funcText = stringutils::rangetoStr(*(Result.SourceManager),
                                                                   func->getSourceRange());
                    size_t funcPos = funcText.find(func->getNameAsString());
                    if (funcPos != std::string::npos) {
                        std::string templateDecl = "template<typename... " + packName + ">\n";
                        funcText.insert(funcPos, templateDecl);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                            func->getSourceRange()), funcText);
                    }
                }
            }
            
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                PD->getSourceRange()), mutatedExpr);
        }
    }
}

void MutatorFrontendAction_211::MutatorASTConsumer_211::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = cxxMemberCallExpr(has(memberExpr()), 
                                                   callee(cxxDestructorDecl())).bind("PseudoDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}