//source file
#include "../include/Mutator_261.h"

// ========================================================================================================
#define MUT261_OUTPUT 1

void MutatorFrontendAction_261::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        //Filter nodes in header files
        if (!TC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TC->getLocation()))
            return;
        if (!TC->isTemplateDecl() || !TC->isCompleteDefinition())
            return;
        
        //Record template class information
        templateClasses.push_back(TC);
        
        //Collect dependent member names from this template class
        for (auto *Decl : TC->decls()) {
            if (auto *TD = dyn_cast<clang::TypeDecl>(Decl)) {
                if (TD->getDeclName().isIdentifier()) {
                    dependentMemberNames.push_back(TD->getNameAsString());
                }
            } else if (auto *VD = dyn_cast<clang::VarDecl>(Decl)) {
                if (VD->isStaticDataMember() && VD->getDeclName().isIdentifier()) {
                    dependentMemberNames.push_back(VD->getNameAsString());
                }
            } else if (auto *AD = dyn_cast<clang::TypeAliasDecl>(Decl)) {
                if (AD->getDeclName().isIdentifier()) {
                    dependentMemberNames.push_back(AD->getNameAsString());
                }
            }
        }
    }
    else if (auto *NC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("NestedClass")) {
        //Filter nodes in header files
        if (!NC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NC->getLocation()))
            return;
        if (!NC->isCompleteDefinition())
            return;
        
        //Find enclosing template class
        const clang::CXXRecordDecl* enclosingTemplate = nullptr;
        for (auto* TC : templateClasses) {
            if (NC->getEnclosingNamespaceContext() == TC) {
                enclosingTemplate = TC;
                break;
            }
        }
        if (!enclosingTemplate)
            return;
        
        //Get template parameters
        auto* Template = enclosingTemplate->getDescribedClassTemplate();
        if (!Template || Template->getTemplateParameters()->size() == 0)
            return;
        
        //Select a dependent member name
        if (dependentMemberNames.empty())
            return;
        size_t idx = getrandom::getRandomIndex(dependentMemberNames.size() - 1);
        std::string memberName = dependentMemberNames[idx];
        
        //Get the source code text of target node
        auto nestedClassText = stringutils::rangetoStr(*(Result.SourceManager),
                                                       NC->getSourceRange());
        
        //Find the opening brace of the nested class
        size_t bracePos = nestedClassText.find('{');
        if (bracePos == std::string::npos)
            return;
        
        //Construct explicit instantiation directive
        std::string templateName = enclosingTemplate->getNameAsString();
        std::string instantiation = "\ntemplate<> typename " + templateName + "<";
        
        //Build template argument list
        auto* Params = Template->getTemplateParameters();
        for (unsigned i = 0; i < Params->size(); ++i) {
            if (i > 0) instantiation += ", ";
            instantiation += Params->getParam(i)->getNameAsString();
        }
        instantiation += ">::" + memberName + ";";
        
        //Perform mutation on the source code text by applying string replacement
        nestedClassText.insert(bracePos + 1, instantiation);
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(NC->getSourceRange()), nestedClassText);
    }
}
  
void MutatorFrontendAction_261::MutatorASTConsumer_261::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass");
    DeclarationMatcher nestedMatcher = cxxRecordDecl(hasParent(cxxRecordDecl(isTemplateInstantiation()))).bind("NestedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(nestedMatcher, &callback);
    matchFinder.matchAST(Context);
}